#ifndef MANDRAKE_REQUEST_H
#define MANDRAKE_REQUEST_H

#include <unordered_map>
#include <string>
#include <vector>

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

static std::vector<HttpMethod> methods_with_body {
    HttpMethod::PATCH,
    HttpMethod::POST,
    HttpMethod::PUT,
};

class HttpRequest {
public:
    HttpMethod method;
    std::string url_path;
    std::string http_version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    [[nodiscard]] static HttpMethod parse_method(std::string &request_method);
};

}
#endif //MANDRAKE_REQUEST_H
