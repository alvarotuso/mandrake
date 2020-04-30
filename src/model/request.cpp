#include <stdexcept>
#include "request.hpp"

namespace mandrake::request {
void HttpRequest::parse_method(std::string &request_method) {
    if (method_names.contains(request_method)) {
        this->method = method_names.at(request_method);
    }
}

void HttpRequest::parse_url(std::string &url) {
    int qs_start = url.find('?');
    unsigned int path_size = qs_start > 0 ? qs_start : url.size();
    this->url_path = url.substr(0, path_size);
    if (qs_start > 0) {
        int p_start = qs_start + 1;
        int p_end = url.find('&', p_start);
        if (p_end == -1) {
            p_end = url.size();
        }
        while (p_start < url.size()) {
            int eq_pos = url.find('=', p_start);
            this->url_parameters.insert({ url.substr(p_start, eq_pos - p_start), url.substr(eq_pos + 1, p_end - eq_pos - 1)});
            p_start = p_end + 1;
            p_end = url.find('&', p_start);
            if (p_end == -1) {
                p_end = url.size();
            }
        }
    }
}
}