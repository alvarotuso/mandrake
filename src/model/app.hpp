#ifndef MANDRAKE_APP_HPP
#define MANDRAKE_APP_HPP
#include <unordered_map>
#include "request.hpp"
#include "response.hpp"

namespace mandrake::app {
    typedef response::HttpResponse resource_t (request::HttpRequest const& request);

    class App {
    public:
        void add_resource(std::string url_pattern, resource_t &resource);
        [[nodiscard]] response::HttpResponse route_request(request::HttpRequest const& request) const;
    private:
        std::unordered_map<std::string, resource_t&> resources;
    };
}

#endif //MANDRAKE_APP_HPP
