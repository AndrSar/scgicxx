#include "include/scgi_protocol_parser.hpp"
#include "include/detail/detail.hpp"

#include <algorithm>
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
            static const string_view SCGI("SCGI");
            static const string_view REQUEST_URI("REQUEST_URI");
            static const string_view CONTENT_LENGTH("CONTENT_LENGTH");
            static const string_view REQUEST_METHOD("REQUEST_METHOD");
            static const string_view CONTENT_TYPE("CONTENT_TYPE");

            auto find = [&tokens](const string_view &str)
            {
                return std::find(tokens.cbegin(), tokens.cend(), SCGI);
            };

            auto scgi_it = find(SCGI);
            if (scgi_it != tokens.cend() && *(scgi_it++) == "1")
            {
                auto request_uri_it = find(REQUEST_URI);
                if (request_uri_it != tokens.cend())
                    request.uri = *(request_uri_it++);

                auto request_method_it = find(REQUEST_METHOD);
                if (request_method_it != tokens.cend())
                    request.set_method(*(request_method_it++));

                if (request.is_post() || request.is_put())
                {
                    auto content_type_it = find(CONTENT_TYPE);
                    if (content_type_it != tokens.cend())
                        request.content_type = *(content_type_it++);

                    auto content_length_it = find(CONTENT_LENGTH);
                    if (content_length_it != tokens.cend())
                    {
                        content_length = atoll((*(content_length_it++)).data());
                    }
                }
            }

            /*std::map<string_view, string_view> map;
            for (std::size_t i = 0; i < k; i += 2)
            {
                map.insert(std::make_pair(tokens[i], tokens[i + 1]));
            }

            std::cout << "Headers: " << std::endl;
            for (const auto &header : map)
            {
                std::cout << std::to_string(header.first) << " : " << std::to_string(header.second) << std::endl;
            }

            auto scgi_it = map.find(string_view("SCGI"));
            if (scgi_it != map.cend() && scgi_it->second == "1")
            {
                auto request_uri_it = map.find(string_view("c"));
                if (request_uri_it != map.cend())
                    request.uri = request_uri_it->second;

                auto content_length_it = map.find(string_view("CONTENT_LENGTH"));
                if (content_length_it != map.cend())
                {
                    content_length = atoll(content_length_it->second.data());
                }

                auto request_method_it = map.find(string_view("REQUEST_METHOD"));
                if (request_method_it != map.cend())
                {
                    request.set_method(request_method_it->second);
                    if (request.is_post() || request.is_put())
                    {
                        auto content_type_it = map.find(string_view("CONTENT_TYPE"));
                        if (content_length_it != map.cend())
                            request.content_type = content_type_it->second;
                    }
                }
            }*/
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
            request.content = array_view_type{content_start_position, content_length};
            result = true;
        }
    }
    else
    {
        result = true;
    }

    return result;
}


http_request scgi_protocol_parser::get_http_request() const
{
    return request;
}




}
}