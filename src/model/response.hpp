#ifndef MANDRAKE_RESPONSE_H
#define MANDRAKE_RESPONSE_H

#include <string>
#include <vector>

namespace mandrake::response {
class HttpResponse {
public:
    int status_code;
    std::string body;

    [[nodiscard]] std::vector<char> to_http() const;
};
}

#endif //MANDRAKE_RESPONSE_H
