//
// Created by alvaro on 29/03/2020.
//

#ifndef MANDRAKE_HTTP_H
#define MANDRAKE_HTTP_H
namespace http {
    class HttpServer {
    public:
        explicit HttpServer(uint16_t);
        int run();
    private:
        uint16_t port = 0;
    };
}
#endif //MANDRAKE_HTTP_H
