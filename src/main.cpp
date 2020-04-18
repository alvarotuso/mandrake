#include <iostream>
#include "server/server.hpp"

int main() {
    mandrake::server::HttpServer server { 8080 };
    return server.run();
}
