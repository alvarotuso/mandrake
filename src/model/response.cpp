#include "response.hpp"
#include "constants.hpp"

namespace mandrake::response {
HttpResponse::HttpResponse(int status_code): status_code{status_code} {}

void HttpResponse::add_header(std::string &response, std::string const& name, std::string const& value) {
    response += name;
    response += constants::header_separator;
    response += value;
    response += constants::crlf;
}

std::string HttpResponse::to_http() const {
    std::string response = this->http_version;
    response += constants::sp;
    response += std::to_string(this->status_code);
    response += constants::sp;
    response +=  constants::status_code_phrases.at(this->status_code);
    response += constants::crlf;
    for(auto const& [name, value] : this->headers) {
        add_header(response, name, value);
    }
    if (!this->body.empty()) {
        add_header(response, constants::content_length_header, std::to_string(this->body.length()));
        response += constants::crlf;
        response += this->body;
    }
    return response;
}
}