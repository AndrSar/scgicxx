#include "../include/server.hpp"


int main(int argn, const char *args[])
{
    asio::io_service io_service;

    scgicxx::server server(io_service);
    server.run("127.0.0.1", 9000);

    io_service.run();

    return 0;
}