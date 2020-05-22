#ifndef MANDRAKE_SERVER_HPP
#define MANDRAKE_SERVER_HPP

#include <vector>

#include "../model/app.hpp"
#include "../model/request.hpp"
#include "../model/response.hpp"

namespace mandrake::server {
class HttpServer {
public:
    explicit HttpServer(mandrake::app::App const& app, uint16_t port);
    [[nodiscard]] int run() const;
private:
    uint16_t port = 0;
    app::App app;
    static void process_request(int socket_descriptor, app::App const& app);
    [[nodiscard]] static request::HttpRequest read_request(int remote_socket_descriptor);
    [[noreturn]] static void read_loop(int socket_descriptor, int thread_number, app::App const& app);
};
}
#endif //MANDRAKE_SERVER_HPP
