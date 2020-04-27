#ifndef MANDRAKE_RESPONSE_H
#define MANDRAKE_RESPONSE_H
#include <unordered_map>
#include <string>
#include <vector>

namespace mandrake::response {
class HttpResponse {
public:
    explicit HttpResponse(int status_code);
    int status_code;
    std::string body;
    std::string http_version;
    std::unordered_map<std::string, std::string> headers;
    [[nodiscard]] std::string to_http() const;
private:
    static void add_header(std::string &response, std::string const& name, std::string const& value);
};
}

#endif //MANDRAKE_RESPONSE_H
