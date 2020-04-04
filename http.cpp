//
// Created by alvaro on 29/03/2020.
//

#include <stdexcept>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "http.h"
#include "sys/socket.h"

namespace http {
    HttpServer::HttpServer(uint16_t port) :port {port} {}
    int HttpServer::run() {
        // TODO: use SOCK_RAW
        // Add SOCK_NONBLOCK flags
        int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1) {
            throw std::runtime_error("Unable to get socket!!");
        }
        std::cout << "Got socket";

        sockaddr_in server {};
        server.sin_port = htons(port);
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;

        if (bind(socket_desc, (sockaddr*) &server, sizeof(server)) == -1) {
            throw std::runtime_error("Unable to bind socket!!");
        }
        std::cout << "Bound socket\n";

        listen(socket_desc, 3);

        std::cout << "Listening for connections\n";

        sockaddr_in client {};
        ulong client_size = sizeof(client);


        int new_socket = accept(socket_desc, (sockaddr*) &client, (socklen_t*) &client_size);
        if (new_socket == -1) {
            throw std::runtime_error("Can't accept connection!!");
        }
        std::string message = "Hello Client, this is my response";
        if (write(new_socket, &message, std::size(message)) == -1) {
            throw std::runtime_error("Can't write");
        }
        return 0;
    }
}
