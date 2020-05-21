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
        node.is_parameter = is_parameter;
    }
    node.resource = resource;
}

response::HttpResponse App::route_request(request::HttpRequest const& request) const {
    ResourceTreeNode* node = this->resource_tree_root;
    for (const std::string& token : utils::iterators::StringSplitGenerator(request.url_path, "/")) {
        if (node->children.contains(token)) {
            node = node->children.at(token);
        } else {
            for (auto const& [token, node] : node->children) {
                if (node->is_parameter) {
                    
                }
            }
        }
    }
    return response::HttpResponse{ 404 };
}
}