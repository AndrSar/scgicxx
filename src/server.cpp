#include "include/server.hpp"
#include "include/detail/detail.hpp"
#include "asio/buffer.hpp"

#include <iostream>
#include <chrono>


namespace scgicxx
{

server::server(asio::io_service &io_service):
    io_service(io_service),
    acceptor(io_service),
    socket(io_service),
    dead_connections_collector_timer(io_service)
{
    launch_dead_connections_collection();
}


server::connection::connection(server::socket_type &&socket):
        socket(std::move(socket)),
        data_timeout_timer(socket.get_io_service(), std::chrono::seconds(2)),
        bytes_received(0)
{
    data_timeout_timer.async_wait([this](std::error_code ec) {
        if (!ec)
        {
            std::cout << "No data or corrupted has been received, connection will be closed" << std::endl;
            this->socket.cancel();
        }
    });
}


void server::run(const std::string &address,
                 const unsigned short port_number,
                 server::handler_function_type func)
{
    handler_function = func;
    asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(address), port_number);
    acceptor.open(endpoint.protocol());
    acceptor.bind(endpoint);
    acceptor.listen();
    accept_connection();
}


void server::accept_connection()
{
    acceptor.async_accept(socket, [this](std::error_code ec)
        {
            if (!ec)
            {
                std::cout << "New connection is established: " << std::endl;

                connections.emplace_back(std::move(socket));
                async_read_data(connections.back());

                std::cout << "Number of connections: " << connections.size() << std::endl;
            }
            else
            {
                std::cout << "Error while accepting new connections: " << ec.message() << std::endl;
            }

            accept_connection();
        });
}


void server::async_read_data(server::connection &conn)
{
    auto handler = [this, &conn](std::error_code ec, std::size_t bytes_transferred)
    {
        if (!ec)
        {
            conn.bytes_received += bytes_transferred;
            std::cout << "Bytes transferred: " << bytes_transferred << std::endl;
            std::cout << "Bytes total: " << conn.bytes_received << std::endl;

            if (conn.stream_parser.parse({conn.buffer.data(), conn.bytes_received}))
            {
                int a = 0;
            }
            else // continue to read data
            {
                this->async_read_data(conn);
            }
        }
        else
        {
            std::cout << "Handler error: " << ec.message() << std::endl;
        }
    };

    auto *ptr = conn.buffer.data() + conn.bytes_received;
    const std::size_t size = conn.buffer.size() - conn.bytes_received;
    conn.socket.async_receive(asio::buffer(ptr, size), handler);
}


void server::launch_dead_connections_collection()
{
    dead_connections_collector_timer.expires_from_now(std::chrono::seconds(60));
    dead_connections_collector_timer.async_wait([this](std::error_code ec){
        if (!ec)
        {
            std::cout << "Removing dead connections... " << std::endl;
            remove_dead_connections();
            launch_dead_connections_collection();
        }
    });
}


void server::remove_dead_connections()
{
    connections.remove_if([](const connection &conn) {
        return !conn.socket.is_open();
    });
}

} // namespace scgicxx
