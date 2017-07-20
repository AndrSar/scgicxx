#include "include/server.hpp"


namespace scgicxx
{

server::server(asio::io_service &io_service):
    io_service(io_service),
    acceptor(io_service),
    socket(io_service)
{

}

void server::run(const std::string &address, const std::string &port)
{
    asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(address),
                                     static_cast<unsigned short>(std::stoul(port)));

    acceptor.open(endpoint.protocol());
    acceptor.bind(endpoint);
    accept_connection();
}


void server::accept_connection()
{
    acceptor.async_accept(socket, [this](std::error_code ec)
        {
           if (!ec)
           {
               //std::make_shared<chat_session>(std::move(socket_), room_)->start();
           }

           accept_connection();
        });
}

} // namespace scgicxx
