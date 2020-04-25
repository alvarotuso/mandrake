#include "response.hpp"

namespace mandrake::response {
const std::string HttpResponse::content_length_header = "Content-Length";

void add_header(std::string &response, std::string const& name, std::string const& value) {
    response += name;
    response += ": ";
    response += value;
    response += "\r\n";
}

HttpResponse::HttpResponse(int status_code): status_code{status_code} {}

std::string HttpResponse::to_http() const {
    std::string response = this->http_version;
    response += " " + std::to_string(this->status_code);
    response += " " + HttpResponse::status_code_phrases.at(this->status_code) + "\r\n";
    for(auto const& [name, value] : this->headers) {
        add_header(response, name, value);
    }
    if (!this->body.empty()) {
        add_header(response, content_length_header, std::to_string(this->body.length()));
        response += "\r\n";
        response += this->body;
    }
    return response;
}

const std::unordered_map<int, std::string> HttpResponse::status_code_phrases {
    { 100, "Continue" },
    { 101, "Switching Protocols" },
    { 200, "OK" },
    { 201, "Created" },
    { 202, "Accepted" },
    { 203, "Non-Authoritative Information" },
    { 204, "No Content" },
    { 205, "Reset Content" },
    { 206, "Partial Content" },
    { 300, "Multiple Choices" },
    { 301, "Moved Permanently" },
    { 302, "Found" },
    { 303, "See Other" },
    { 304, "Not Modified" },
    { 305, "Use Proxy" },
    { 307, "Temporary Redirect" },
    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 402, "Payment Required" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
    { 405, "Method Not Allowed" },
    { 406, "Not Acceptable" },
    { 407, "Proxy Authentication Required" },
    { 408, "Request Time-out" },
    { 409, "Conflict" },
    { 410, "Gone" },
    { 411, "Length Required" },
    { 412, "Precondition Failed" },
    { 413, "Request Entity Too Large" },
    { 414, "Request-URI Too Large" },
    { 415, "Unsupported Media Type" },
    { 416, "Requested range not satisfiable" },
    { 417, "Expectation Failed" },
    { 500, "Internal Server Error" },
    { 501, "Not Implemented" },
    { 502, "Bad Gateway" },
    { 503, "Service Unavailable" },
    { 504, "Gateway Time-out" },
    { 505, "HTTP Version not supported" }
};
}