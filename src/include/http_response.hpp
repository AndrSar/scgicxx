#pragma once

#include <cstddef>
#include <vector>


namespace scgicxx
{

class http_response
{
public:
    static http_response ok(const char *content_type, const char *content = nullptr);
    static http_response forbidden();
    static http_response not_found();

public:
    http_response()
    {
        buffer.reserve(1024);
    }

    std::size_t size() const
    {
        return buffer.size();
    }

    const char *data() const
    {
        return buffer.data();
    }

    void reserve(std::size_t n)
    {
        buffer.reserve(n);
    }

    void write(const char *str);
    void write(const char *data, std::size_t n);
    void writeln(const char *str);

private:
    void add_connection_close();

private:
    std::vector<char> buffer;
};

}
