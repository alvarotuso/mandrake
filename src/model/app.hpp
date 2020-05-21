#ifndef MANDRAKE_APP_HPP
#define MANDRAKE_APP_HPP
#include <unordered_map>
#include "request.hpp"
#include "response.hpp"

namespace mandrake::app {
typedef response::HttpResponse resource_t (request::HttpRequest const& request);

struct ResourceTreeNode {
    std::unordered_map<std::string, ResourceTreeNode*> children;
    resource_t* resource = nullptr;
    bool is_parameter = false;

    explicit ResourceTreeNode() = default;
    ~ResourceTreeNode();
};

class App {
public:
    explicit App();
    ~App();
    void add_resource(std::string url_pattern, resource_t &resource);
    [[nodiscard]] response::HttpResponse route_request(request::HttpRequest const& request) const;
private:
    ResourceTreeNode* resource_tree_root;
};
}

#endif //MANDRAKE_APP_HPP
