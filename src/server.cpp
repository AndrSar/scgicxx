
//          Copyright Andrey Lifanov 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "include/server.hpp"
#include "include/detail/detail.hpp"
#include "asio/buffer.hpp"

#include <iostream>


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
    connection_init_time = std::chrono::system_clock::now();
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

    asio::ip::tcp::no_delay no_delay_option(true);
    acceptor.set_option(no_delay_option);

    asio::socket_base::keep_alive keep_alive_option(false);
    acceptor.set_option(keep_alive_option);

    acceptor.bind(endpoint);
    acceptor.listen();
    accept_connection();
}


void server::stop()
{
    acceptor.cancel();
    acceptor.close();
}


void server::accept_connection()
{
    acceptor.async_accept(socket, [this](std::error_code ec)
        {
            bool operation_has_been_aborted = false;

            if (!ec)
            {
                std::cout << "New connection is established: " << std::endl;

                connections.emplace_back(std::move(socket));
                async_read_data(connections.back());

                std::cout << "Number of connections: " << connections.size() << std::endl;
            }
            else if (ec.value() == asio::error::operation_aborted)
            {
                operation_has_been_aborted = true;
                std::cout << "Abort accepting new connections: " << ec.message() << std::endl;
            }
            else
            {
                std::cout << "Error while accepting new connections: " << ec.message() << std::endl;
            }

            if (!operation_has_been_aborted)
            {
                accept_connection();
            }
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

//            for (std::size_t i = 0; i < 1024; ++i)
//            {
//                std::cout << conn.buffer[i];
//            }
//            std::cout << std::endl;

            if (conn.stream_parser.parse({conn.buffer.data(), conn.bytes_received}))
            {
                conn.data_timeout_timer.cancel();
                this->process_request(conn, conn.stream_parser.get_http_request());
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


void server::process_request(server::connection &conn, const http_request &req)
{
    auto response = handler_function(req);

    auto handler = [this, &conn](std::error_code ec, std::size_t bytes_transferred)
    {
        if (!ec)
        {
            this->close_connection(conn);
        }
        else
        {
            std::cout << "Error while sending response: " << ec.message() << std::endl;
        }
    };

    asio::async_write(conn.socket, asio::buffer(response.data(), response.size()), handler);
}


void server::close_connection(server::connection &conn)
{
    const auto time_diff = std::chrono::system_clock::now() - conn.connection_init_time;
    conn.processing_time = std::chrono::duration_cast<std::chrono::microseconds>(time_diff);

    std::cout << "Took " << conn.processing_time.count() << " microseconds." << std::endl;

    conn.socket.shutdown(asio::socket_base::shutdown_both);
    conn.socket.close();
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
