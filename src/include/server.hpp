
//          Copyright Andrey Lifanov 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#pragma once

#include "asio.hpp"


namespace scgicxx
{

class server
{
public:
    server(asio::io_service &io_service);
    void run(const std::string &adsress, const std::string &port);

private:
    void accept_connection();

private:
    asio::io_service &io_service;
    asio::ip::tcp::acceptor acceptor;
    asio::ip::tcp::socket socket;
};

} // namespace scgicxx

