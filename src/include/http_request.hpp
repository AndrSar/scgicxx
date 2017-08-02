#pragma once

#include "detail/string_view.hpp"


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

    method_enum method;
    string_view uri;
    array_view<const char> content;
};

}
