#ifndef MANDRAKE_APP_HPP
#define MANDRAKE_APP_HPP
#include <unordered_map>
#include <memory>
#include "request.hpp"
#include "response.hpp"

namespace mandrake::app {
typedef response::HttpResponse resource_t (request::HttpRequest const& request);

struct ResourceTreeNode {
    std::unordered_map<std::string, std::shared_ptr<ResourceTreeNode>> children;
    resource_t* resource = nullptr;
    bool is_variable = false;
};

struct RequestRouteResult {
    resource_t* resource = nullptr;
    std::unordered_map<std::string, std::string> url_variables;
    explicit RequestRouteResult(resource_t* resource);
};

class App {
public:
    explicit App();
    void add_resource(std::string url_pattern, resource_t &resource);
    [[nodiscard]] response::HttpResponse route_request(request::HttpRequest request) const;
private:
    std::shared_ptr<ResourceTreeNode> resource_tree_root;
    [[nodiscard]] std::optional<RequestRouteResult> route_with_variables(
            std::shared_ptr<ResourceTreeNode> const& current_node, std::string remaining_path) const;
};
}

#endif //MANDRAKE_APP_HPP
