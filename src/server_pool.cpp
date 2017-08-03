
//          Copyright Andrey Lifanov 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "include/server_pool.hpp"

#include <iostream>


namespace scgicxx
{

server_pool::server_pool(const std::size_t n,
                         const std::string &address,
                         const unsigned short initial_port_number,
                         handler_function_type requests_handler_function)
{
    unsigned short port_number = initial_port_number;
    std::size_t amount = n;
    while (amount > 0)
    {
        add_server(address, port_number, requests_handler_function);
        ++port_number;
        --amount;
    }
}


void server_pool::add_server(const std::string &address,
                const unsigned short port_number,
                handler_function_type requests_handler_function)
{
    servers_info.push_back({address, port_number, requests_handler_function});
}


void server_pool::run()
{
    asio::io_service io_service;
    asio::signal_set signals(io_service, SIGINT, SIGTERM);

    auto signals_handler = [this](const asio::error_code& error, int signal_number)
    {
        if (!error)
        {
            std::cout << "Signal " << signal_number << " occurred. Program will be stopped." << std::endl;
            this->stop();
        }
    };

    signals.async_wait(signals_handler);

    for (const auto &info : servers_info)
    {
        auto s = std::make_shared<server>(io_service);
        servers.push_back(s);

        threads.emplace_back([&io_service, s, info](){
            s->run(info.address, info.port_number, info.requests_handler_function);
            io_service.run();
        });

        threads.back().detach();
    }

    while(!io_service.stopped())
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}


void server_pool::stop()
{
    for (auto &server: servers)
    {
        server->stop();
    }
}

}
