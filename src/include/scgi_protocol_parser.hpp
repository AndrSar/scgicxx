#pragma once

#include "detail/string_view.hpp"
#include "http_request.hpp"
#include <cstddef>
#include <vector>


namespace scgicxx
{
namespace detail
{

class scgi_protocol_parser
{
    enum class state_enum
    {
        initial = 0,
        headers_received = 1,
        content_received = 2
    };

public:
    using array_view_type = array_view<const char>;

public:
    scgi_protocol_parser(): state(state_enum::initial),
                            content_length(0)
    {
    }

    bool parse(array_view_type buffer);
    http_request get_http_request() const;

private:
    bool parse_headers(const array_view_type);
    bool locate_content(array_view_type);

private:
    state_enum state;
    std::size_t content_length;
    string_view headers_netstring;
    http_request request;
};

}
}
