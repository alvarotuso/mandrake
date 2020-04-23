#ifndef MANDRAKE_RESPONSE_H
#define MANDRAKE_RESPONSE_H

#include <unordered_map>
#include <string>
#include <vector>

namespace mandrake::response {
class HttpResponse {
public:
    int status_code;
    std::string body;
    std::string http_version;
    std::unordered_map<std::string, std::string> headers;
    [[nodiscard]] std::string to_http() const;
private:
    static const std::unordered_map<int, std::string> status_code_phrases;
    static const std::string content_length_header;
};
}

#endif //MANDRAKE_RESPONSE_H
