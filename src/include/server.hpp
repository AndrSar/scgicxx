
//          Copyright Andrey Lifanov 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#define ASIO_STANDALONE
#define ASIO_HAS_STD_ARRAY
#include "asio.hpp"

#include "scgi_protocol_parser.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "logging/logger.hpp"

#include <memory>
#include <list>
#include <array>
#include <functional>
#include <chrono>


namespace scgicxx
{

class server
{
    using self = server;
public:
    using handler_function_type = std::function<http_response(const http_request&)>;
    using socket_type = asio::ip::tcp::socket;

    struct connection
    {
        explicit connection(socket_type &&socket);

        socket_type socket;
        std::array<char, 2048UL> buffer;
        std::size_t bytes_received;
        asio::steady_timer data_timeout_timer;
        detail::scgi_protocol_parser stream_parser;
        std::chrono::system_clock::time_point connection_init_time;
        std::chrono::microseconds processing_time;
    };

public:
    explicit server(asio::io_service &io_service);
    void run(const std::string &address,
             const unsigned short port_number,
             handler_function_type handler_function);

    void stop();

    server(const self &another) = delete;
    server(self &&another) = delete;
    self &operator=(const self &another) = delete;
    self &operator=(self &&another) = delete;
    ~server() = default;

private:
    void accept_connection();
    void async_read_data(connection &conn);
    void process_request(connection &conn, const http_request &req);
    void close_connection(connection &conn);
    void launch_dead_connections_collection();
    void remove_dead_connections();

private:
    asio::io_service &io_service;
    asio::ip::tcp::acceptor acceptor;
    socket_type socket;
    handler_function_type handler_function;

    std::list<connection> connections;
    asio::steady_timer dead_connections_collector_timer;
};

} // namespace scgicxx

