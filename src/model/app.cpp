#include <iostream>
#include "app.hpp"
#include "response.hpp"

namespace mandrake::app {
    void App::add_resource(std::string url_pattern, resource_t &resource) {
        this->resources.insert({std::move(url_pattern), resource});
    }

    response::HttpResponse App::route_request(request::HttpRequest const& request) const {
        if (this->resources.contains(request.url_path)) {
            return this->resources.at(request.url_path)(request);
        }
        return response::HttpResponse{ 404 };
    }
}