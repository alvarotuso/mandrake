#include <iostream>
#include <iomanip>
#include <sys/socket.h>
#include "http.h"

int main() {
    http::HttpServer server { 8080 };
    server.run();
}
