#include "../include/server_pool.hpp"

#include <iostream>


int main(int argn, const char *args[])
{
    auto logger = std::make_shared<scgicxx::logging::logger>();

    auto requests_handler = [](const scgicxx::http_request &request) {
        return scgicxx::http_response::ok("text/plain", "Hello!");
    };

    scgicxx::server_pool server_pool(1, "127.0.0.1", 9000, requests_handler, logger);
    server_pool.run();

    return 0;
}