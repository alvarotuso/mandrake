#include <iostream>
#include "server/server.hpp"
#include "model/response.hpp"

mandrake::response::HttpResponse ping(mandrake::request::HttpRequest const& request) {
    auto response = mandrake::response::HttpResponse { 200 };
    response.body = R"({"status": "ok"})";
    response.headers.insert({ "Content-Type", "application/json" });
    return response;
}

mandrake::response::HttpResponse variables(mandrake::request::HttpRequest const& request) {
    auto response = mandrake::response::HttpResponse { 200 };
    response.body = R"({"status": "ok", "variables": ")";
    for (auto const& [variable, value] : request.url_variables) {
        response.body += variable;
        response.body += ": ";
        response.body += value;
        response.body += "\n";
    }
    response.body += R"("})";
    response.headers.insert({ "Content-Type", "application/json" });
    return response;
}

int main() {
    mandrake::app::App app {};
    app.add_resource("/ping", ping);
    app.add_resource("/variables/<variable1>/fixed_token/<variable2>", variables);
    mandrake::server::HttpServer server { app, 8080 };
    return server.run();
}
