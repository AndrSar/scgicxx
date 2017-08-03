#include "../include/detail/detail.hpp"
#include <cstdlib>


namespace scgicxx
{
namespace detail
{

const string_view parse_net_string(const array_view<const char> data)
{
    string_view result;
    std::size_t net_string_size = 0;

    std::size_t colon_index = data.find(':');
    if (colon_index != array_view<char>::npos)
    {
        net_string_size = static_cast<std::size_t>(std::atol(data.data()));
    }

    if (net_string_size > 0 && net_string_size <= (data.size() - colon_index))
    {
        result = string_view{data.data() + colon_index + 1, net_string_size};
    }

    return result;
}

}
}