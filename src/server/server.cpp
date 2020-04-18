#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <zconf.h>
#include <vector>
#include <thread>

#include "server.hpp"
#include "../model/request.hpp"
#include "../model/response.hpp"

namespace mandrake::server {

const auto buffer_size = 1024;
const auto max_request_size = 1024 * 1024 * 1024;  // 1GB

HttpServer::HttpServer(uint16_t port) : port{port} {}

void HttpServer::process_request(int remote_socket_descriptor) {
    request::HttpRequest request = HttpServer::read_request(remote_socket_descriptor);
    std::vector<char> response = response::HttpResponse { .status_code =  200, .body =  "hello" }.to_http();
    if (write(remote_socket_descriptor, response.data(), response.size()) == -1) {
        throw std::runtime_error("Can't write");
    }
}

request::HttpRequest HttpServer::read_request(int remote_socket_descriptor) {
    std::vector<char> request_data;
    std::vector<char> buffer(buffer_size);
    while (read(remote_socket_descriptor, buffer.data(), buffer.size()) > 0) {
        request_data.insert(request_data.end(), buffer.begin(), buffer.end());
    }
    return request::HttpRequest::from_http(request_data);
}

[[noreturn]] void HttpServer::read_loop(int socket_descriptor, int thread_number) {
    std::cout << "Initialized thread " << thread_number;
    while (true) {
        sockaddr_in client{};
        ulong client_size = sizeof(client);
        int remote_socket = accept(socket_descriptor, (sockaddr *) &client, (socklen_t *) &client_size);
        if (remote_socket > 0) {
            std::cout << "Received connection in thread " << thread_number;
            HttpServer::process_request(remote_socket);
            close(socket_descriptor);
        }
    }
}

int HttpServer::run() const {
    // TODO: use SOCK_RAW
    // Add SOCK_NONBLOCK flags
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        throw std::runtime_error("Unable to get socket!!");
    }

    sockaddr_in server{};
    server.sin_port = htons(this->port);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_desc, (sockaddr *) &server, sizeof(server)) == -1) {
        throw std::runtime_error("Unable to bind socket!!");
    }

    listen(socket_desc, 3);

    std::cout << "Listening for connections";

    unsigned int threads = std::thread::hardware_concurrency();
    std::vector<std::shared_ptr<std::thread>> request_handlers;
    for (auto i = 0; i < threads; i++) {
        auto request_handler = std::make_shared<std::thread>(HttpServer::read_loop, socket_desc, i);
        request_handlers.emplace_back(request_handler);
    }
    for (auto const& request_handler : request_handlers) {
        request_handler->join();
    }
    return 0;
}
}
