#pragma once

#include "array_view.hpp"
#include "string_view.hpp"
#include <cstddef>
#include <array>
#include <vector>


namespace scgicxx
{
namespace detail
{

const string_view parse_net_string(const array_view<const char> data);


template <class T>
std::size_t split_into_tokens(const basic_string_view<T> str,
                              std::vector<basic_string_view<T>> &result,
                              T separator)
{
    std::size_t k = 0;
    long last_separator = str.front() == separator ? 0 : -1;
    std::size_t non_separators_counter = 0;

    for (std::size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] != separator)
        {
            ++non_separators_counter;
        }
        else
        {
            if (non_separators_counter > 0)
            {
                result.emplace_back(str.data() + last_separator + 1, non_separators_counter);
                ++k;
                non_separators_counter = 0;
            }

            last_separator = static_cast<long>(i);
        }
    }

    return k;
}

}
}
