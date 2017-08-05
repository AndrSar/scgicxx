
//          Copyright Andrey Lifanov 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "server.hpp"
#include <cstddef>
#include <vector>
#include <memory>


namespace scgicxx
{

class server_pool
{
    using self = server_pool;
public:
    using handler_function_type = server::handler_function_type;

    struct server_info
    {
        std::string address;
        unsigned short port_number;
        handler_function_type requests_handler_function;
    };

public:
    explicit server_pool(const std::shared_ptr<logging::logger> &logger);

    server_pool(const std::size_t n,
                const std::string &address,
                const unsigned short initial_port_number,
                handler_function_type requests_handler_function,
                const std::shared_ptr<logging::logger> &logger);


    void add_server(const std::string &address,
                    const unsigned short port_number,
                    handler_function_type requests_handler_function);

    void run();

    server_pool(const self &another) = delete;
    server_pool(self &&another) = delete;
    self &operator=(const self &another) = delete;
    self &operator=(self &&another) = delete;
    ~server_pool() = default;

private:
    void stop();

private:
    //std::vector<std::thread> threads;
    std::vector<server_info> servers_info;
    std::vector<std::shared_ptr<server>> servers;
    std::shared_ptr<logging::logger> logger;
};

}