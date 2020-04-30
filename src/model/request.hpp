#ifndef MANDRAKE_REQUEST_H
#define MANDRAKE_REQUEST_H

#include <unordered_map>
#include <string>
#include <set>

namespace mandrake::request {
enum HttpMethod {
    CONNECT,
    GET,
    HEAD,
    OPTIONS,
    PATCH,
    POST,
    PUT,
    TRACE,
};

static const std::set<HttpMethod> methods_with_body {
    HttpMethod::PATCH,
    HttpMethod::POST,
    HttpMethod::PUT,
};

static const std::unordered_map<std::string, HttpMethod> method_names {
    { "CONNECT", HttpMethod::CONNECT },
    { "GET", HttpMethod::GET },
    { "HEAD", HttpMethod::HEAD },
    { "OPTIONS", HttpMethod::OPTIONS },
    { "PATCH", HttpMethod::PATCH },
    { "POST", HttpMethod::POST },
    { "PUT", HttpMethod::PUT },
    { "TRACE", HttpMethod::TRACE },
};

class HttpRequest {
public:
    HttpMethod method;
    std::string url_path;
    std::string http_version;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> url_parameters;
    std::string body;

    void parse_method(std::string &request_method);
    void parse_url(std::string &url);
};

}
#endif //MANDRAKE_REQUEST_H
