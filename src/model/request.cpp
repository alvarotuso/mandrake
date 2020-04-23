#include <stdexcept>
#include "request.hpp"

namespace mandrake::request {
HttpMethod HttpRequest::parse_method(std::string &request_method) {
    if (request_method == "CONNECT") {
        return HttpMethod::CONNECT;
    } else if (request_method == "GET") {
        return HttpMethod::GET;
    } else if (request_method == "HEAD") {
        return HttpMethod::HEAD;
    } else if (request_method == "OPTIONS") {
        return HttpMethod::OPTIONS;
    } else if (request_method == "PATCH") {
        return HttpMethod::PATCH;
    } else if (request_method == "POST") {
        return HttpMethod::POST;
    } else if (request_method == "PUT") {
        return HttpMethod::PUT;
    } else {
        return HttpMethod::TRACE;
    }
}
}