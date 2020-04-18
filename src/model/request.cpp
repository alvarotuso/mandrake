#include <stdexcept>
#include "request.hpp"

namespace mandrake::request {
HttpRequest HttpRequest::from_http(std::vector<char> request) {
    throw std::runtime_error(std::string { request.data() });
}
}