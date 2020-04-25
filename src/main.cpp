#include <iostream>
#include "server/server.hpp"
#include "model/response.hpp"

mandrake::response::HttpResponse ping(mandrake::request::HttpRequest const& request) {
    auto response = mandrake::response::HttpResponse { 200 };
    response.body = R"({"status": "ok"})";
    response.headers.insert({ "Content-Type", "application/json" });
    return response;
}

int main() {
    mandrake::app::App app;
    app.add_resource("/ping", ping);
    mandrake::server::HttpServer server { app, 8080 };
    return server.run();
}
