#include "response.hpp"

namespace mandrake::response {
std::vector<char> HttpResponse::to_http() const {
    return std::vector<char>{ 0 };
}
}