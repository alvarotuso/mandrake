//
// Created by alvaro on 12/04/2020.
//

#ifndef MANDRAKE_REQUEST_H
#define MANDRAKE_REQUEST_H

#include <string>
#include <vector>

namespace mandrake::request {
enum HttpMethod {
    get,
    post,
    put,
    patch,
};

class HttpRequest {
public:
    HttpMethod method;
    std::string url;

    static HttpRequest from_http(std::vector<char> request);
};

}
#endif //MANDRAKE_REQUEST_H
