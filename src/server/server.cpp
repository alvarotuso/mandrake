#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <zconf.h>
#include <vector>
#include <thread>

#include "server.hpp"
#include "../model/constants.hpp"

namespace mandrake::server {

const auto buffer_size = 1024;
const auto max_request_size = 1024 * 1024 * 1024;  // 1GB

// Get the next message line from the buffer
std::string get_next_message_line(std::string &buffer, unsigned int start) {
    int new_line_end = buffer.find("\r\n", start);
    return buffer.substr(start, new_line_end - start);
}

// Populate the headers map and get the location of the next line
unsigned int populate_headers(std::unordered_map<std::string, std::string> &headers,
        std::string &buffer, unsigned int start) {
    bool empty_line_found = false;
    while (!empty_line_found) {
        std::string header_line = get_next_message_line(buffer, start);
        start += header_line.size() + 2;
        if (header_line.empty()) {
            empty_line_found = true;
        } else {
            int title_split = header_line.find(':');
            headers.insert({
                                   header_line.substr(0, title_split),
                                   header_line.substr(title_split + 2, header_line.size() - title_split)
                           });
        }
    }
    return start;
}

HttpServer::HttpServer(mandrake::app::App app, uint16_t port): port{port}, app{std::move(app)} {}

void HttpServer::process_request(int remote_socket_descriptor, app::App const& app) {
    request::HttpRequest request = HttpServer::read_request(remote_socket_descriptor);
    auto http_response = app.route_request(request);
    http_response.http_version = request.http_version;
    std::string response = http_response.to_http();
    if (write(remote_socket_descriptor, response.data(), response.size()) == -1) {
        throw std::runtime_error("Can't write");
    }
}

request::HttpRequest HttpServer::read_request(int remote_socket_descriptor) {
    std::string buffer(buffer_size, 0);
    int read_bytes = read(remote_socket_descriptor, buffer.data(), buffer.size());
    if (read_bytes == -1) {
        throw std::runtime_error("Unable to read socket");
    }
    std::string request_line = get_next_message_line(buffer, 0);
    int separator1 = request_line.find(constants::sp);
    int separator2 = request_line.find(constants::sp, separator1 + 1);
    std::string http_method = request_line.substr(0, separator1);
    std::string url_path = request_line.substr(separator1 + 1, separator2 - separator1 - 1);
    std::string http_version = request_line.substr(separator2 + 1, request_line.size() - 1);

    std::unordered_map<std::string, std::string> headers;
    unsigned int headers_start = request_line.size() + 2;
    unsigned int headers_end = populate_headers(headers, buffer, headers_start);

    if (headers.contains(constants::content_length_header)) {

    }

    request::HttpRequest req;
    req.method = request::HttpRequest::parse_method(http_method);
    req.http_version = http_version;
    req.headers = headers;
    req.url_path = url_path;
    return req;
}

[[noreturn]] void HttpServer::read_loop(int socket_descriptor, int thread_number, app::App const& app) {
    std::cout << "Initialized thread " << thread_number << std::endl;
    while (true) {
        sockaddr_in client{};
        ulong client_size = sizeof(client);
        int remote_socket = accept(socket_descriptor, (sockaddr *) &client, (socklen_t *) &client_size);
        if (remote_socket > 0) {
            std::cout << "Received connection in thread " << thread_number << std::endl;
            HttpServer::process_request(remote_socket, app);
            close(remote_socket);
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

    std::cout << "Listening for connections" << std::endl;

    unsigned int threads = std::thread::hardware_concurrency();
    std::vector<std::shared_ptr<std::thread>> request_handlers;
    for (auto i = 0; i < threads; i++) {
        auto request_handler = std::make_shared<std::thread>(this->read_loop, socket_desc, i, this->app);
        request_handlers.emplace_back(request_handler);
    }
    for (auto const& request_handler : request_handlers) {
        request_handler->join();
    }
    return 0;
}
}
