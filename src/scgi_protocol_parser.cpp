#include "include/scgi_protocol_parser.hpp"
#include "include/detail/detail.hpp"

#include <map>
#include <iostream>


namespace scgicxx
{
namespace detail
{

bool scgi_protocol_parser::parse(scgi_protocol_parser::array_view_type buffer)
{
    bool result = false;

    if (state == state_enum::initial)
    {
        if (this->parse_headers(buffer))
        {
            state = state_enum::headers_received;
        }
    }

    if (state == state_enum::headers_received)
    {
        if (this->locate_content(buffer))
        {
            state = state_enum::content_received;
        }
    }

    if (state == state_enum::content_received)
    {
        result = true;
    }

    return result;
}


bool scgi_protocol_parser::parse_headers(const scgi_protocol_parser::array_view_type buffer)
{
    bool result = false;
    headers_netstring = detail::parse_net_string(buffer);
    if (!headers_netstring.empty())
    {
        std::vector<string_view> tokens;
        tokens.reserve(64);
        const std::size_t k = detail::split_into_tokens(headers_netstring, tokens, '\0');

//        for (const auto &token : tokens)
//        {
//            std::cout << std::to_string(token) << std::endl;
//        }

        if (k > 0)
        {
            std::map<string_view, string_view> map;
            for (std::size_t i = 0; i < k; i += 2)
            {
                map.insert(std::make_pair(tokens[i], tokens[i + 1]));
            }

//            for (const auto &header : map)
//            {
//                std::cout << std::to_string(header.first) << " : " << std::to_string(header.second) << std::endl;
//            }

            auto it = map.find(string_view("CONTENT_LENGTH"));
            if (it != map.cend())
            {
                content_length = atoll(it->second.data());
            }
        }

        result = true;
    }

    return result;
}


bool scgi_protocol_parser::locate_content(const scgi_protocol_parser::array_view_type buffer)
{
    bool result = false;

    if (content_length > 0)
    {
        const char * const content_start_position = headers_netstring.cend() + 1;
        if (content_length == buffer.cend() - content_start_position)
        {
            content = array_view_type{content_start_position, content_length};
            result = true;
        }
    }
    else
    {
        result = true;
    }

    return result;
}

}
}