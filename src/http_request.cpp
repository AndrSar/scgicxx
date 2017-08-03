#include "include/http_request.hpp"

namespace scgicxx
{

void http_request::set_method(const string_view &str)
{
    if (str == "GET")
    {
        method = GET;
    }
    else if (str == "POST")
    {
        method = POST;
    }
    else if (str == "PUT")
    {
        method = PUT;
    }
    else if (str == "DELETE")
    {
        method = DELETE;
    }
}

}

