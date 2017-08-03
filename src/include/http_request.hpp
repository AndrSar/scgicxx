#pragma once

#include "detail/string_view.hpp"
#undef DELETE

namespace scgicxx
{

struct http_request
{
    enum method_enum
    {
        UNKNOWN = 0,
        GET,
        POST,
        PUT,
        DELETE
    };

    http_request(): method(UNKNOWN)
    {
    }

    void set_method(const string_view &str);

    bool is_get() const
    {
        return method == GET;
    }

    bool is_post() const
    {
        return method == POST;
    }

    bool is_put() const
    {
        return method == PUT;
    }

    method_enum method;
    string_view uri;
    string_view content_type;
    array_view<const char> content;
};

}
