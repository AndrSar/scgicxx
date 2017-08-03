#include "include/http_response.hpp"


namespace scgicxx
{

http_response http_response::ok(const char *content_type, const char *content)
{
    http_response response;
    response.writeln("Status: 200 OK");
    response.add_connection_close();
    response.write("Content-Type: ");
    response.writeln(content_type);

    if (content)
    {
        response.writeln("");
        response.write(content);
    }

    return response;
}


http_response http_response::forbidden()
{
    http_response response;
    response.writeln("Status: 403 Forbidden");
    response.add_connection_close();
    return response;
}


http_response http_response::not_found()
{
    http_response response;
    response.writeln("Status: 404 Not Found");
    response.add_connection_close();
    return response;
}


void http_response::write(const char *str)
{
    std::size_t i = 0;
    while (str[i] != '\0')
    {
        buffer.push_back(str[i]);
        ++i;
    }
}


void http_response::write(const char *data, std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        buffer.push_back(data[i]);
    }
}


void http_response::writeln(const char *str)
{
    this->write(str);
    buffer.push_back(0x0d);
    buffer.push_back(0x0a);
}


void http_response::add_connection_close()
{
    writeln("Connection: close");
}

}