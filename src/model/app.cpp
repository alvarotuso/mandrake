#include <iostream>
#include "app.hpp"
#include "response.hpp"
#include "../utils/iterators.hpp"

namespace mandrake::app {
App::App() {
    this->resource_tree_root = new ResourceTreeNode {};
}

App::~App() {
    delete this->resource_tree_root;
}

ResourceTreeNode::~ResourceTreeNode() {
    for (auto const& [token, node] : this->children) {
        delete node;
    }
}

void App::add_resource(std::string url_pattern, resource_t &resource) {
    ResourceTreeNode node = *this->resource_tree_root;
    for (std::string token : utils::iterators::StringSplitGenerator(std::move(url_pattern), "/")) {
        if (token.empty()) {
            continue;
        }
        bool is_parameter = false;
        if (token.starts_with('<') && token.ends_with('>')) {
            is_parameter = true;
            token = token.substr(1, token.size() - 2);
        }
        if (!node.children.contains(token)) {
            auto new_node = new ResourceTreeNode {};
            node.children.insert({token, new_node});
        }
        node = *node.children.at(token);
        node.is_variable = is_parameter;
    }
    node.resource = resource;
}

RequestRouteResult::RequestRouteResult(resource_t* resource): resource {resource} {}

std::optional<RequestRouteResult> App::route_with_variables(ResourceTreeNode* current_node, std::string remaining_path) const {
    int end = remaining_path.find('/');
    std::string token;
    if (end < 0) {
        token = remaining_path.substr(0, remaining_path.length());
        remaining_path = "";
    } else {
        token = remaining_path.substr(0, end);
        remaining_path = remaining_path.substr(end + 1);
    }
    if (current_node->children.contains(token)) {
        current_node = current_node->children.at(token);
        if (remaining_path.empty()) {
            return std::optional<RequestRouteResult> { RequestRouteResult { current_node->resource } };
        } else {
            return this->route_with_variables(current_node, remaining_path);
        }
    } else {
        for (auto const& [variable_name, node] : current_node->children) {
            if (node->is_variable) {
                std::optional<RequestRouteResult> result = this->route_with_variables(node, remaining_path);
                if (result.has_value()) {
                    result->url_variables.insert({variable_name, token});
                    return result;
                }
            }
        }
        return std::nullopt;
    }
}

response::HttpResponse App::route_request(request::HttpRequest request) const {
    std::string url_path = request.url_path;
    if (url_path.ends_with('/')) {
        url_path = url_path.substr(0, url_path.length() - 1);
    }
    std::optional<RequestRouteResult> route_result = this->route_with_variables(this->resource_tree_root, url_path);
    if (route_result.has_value()) {
        request.url_variables = route_result.value().url_variables;
        return route_result->resource(request);
    }
    return response::HttpResponse{ 404 };
}
}