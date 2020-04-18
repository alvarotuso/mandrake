
#ifndef MANDRAKE_SERVER_HPP
#define MANDRAKE_SERVER_HPP

#include <vector>

#include "../model/request.hpp"
#include "../model/response.hpp"

namespace mandrake::server {
class HttpServer {
public:
    explicit HttpServer(uint16_t port);
    [[nodiscard]] int run() const;
private:
    uint16_t port = 0;
    void static process_request(int socket_descriptor);
    [[nodiscard]] static request::HttpRequest read_request(int remote_socket_descriptor);

    [[noreturn]] static void read_loop(int socket_descriptor, int thread_number);
};
}
#endif //MANDRAKE_SERVER_HPP
