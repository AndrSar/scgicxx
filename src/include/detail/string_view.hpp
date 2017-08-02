#pragma once

#include "array_view.hpp"
#include <cstring>
#include <algorithm>
#include <string>


template <class CharT>
class basic_string_view;

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

template <class CharT>
class basic_string_view : public array_view<const CharT>
{
    using self = basic_string_view;
    using parent_type = array_view<const CharT>;

public:
    using value_type =      typename parent_type::value_type;
    using pointer =         typename parent_type::pointer;
    using const_pointer =   typename parent_type::const_pointer;
    using reference =       typename parent_type::reference;
    using const_reference = typename parent_type::const_reference;
    using size_type =       typename parent_type::size_type;
    using difference_type = typename parent_type::difference_type;

    using iterator = pointer;
    using const_iterator = const_pointer;
    static constexpr size_type npos = parent_type::npos;

public:
    basic_string_view() noexcept : parent_type()
    {
    }

    basic_string_view(const CharT *s, size_type count) noexcept
    {
        this->_data = s;
        this->_size = count;
    }

    explicit basic_string_view(const CharT *s) noexcept
    {
        this->_data = s;
        this->_size = std::strlen(s);
    }

    explicit basic_string_view(const parent_type& array_view) noexcept
    {
        this->_data = array_view._data;
        this->_size = array_view._size;
    }

    self substr(size_type position, size_type count) const
    {
        return {this->_data + position, count};
    }

    size_type find(const CharT *s, size_type position = 0) const noexcept
    {
        size_type result = npos;

        std::size_t j = 0;
        for (std::size_t i = position; i < this->size(); ++i)
        {
            if (this->_data[i] == s[j])
            {
                if (s[j + 1] == '\0')
                {
                    //result = {haystack.data() + (i - j), j + 1};
                    result = i - j;
                    break;
                }

                ++j;
            }
            else
            {
                j = 0;
            }
        }

        return result;
    }

    bool operator==(const self &another) const
    {
        if (this->size() == another.size() && this->data() == another.data())
            return true;

        if (this->size() != another.size())
            return false;

        for (size_type i = 0; i < this->size(); ++i)
        {
            if (this->_data[i] != another._data[i])
            {
                return false;
            }
        }

        return true;
    }

    bool operator==(const CharT *s) const
    {
        return *this == self{s};
    }

    bool operator!=(const self &another) const
    {
        return !(*this == another);
    }

    bool operator<(const self &another) const
    {
        return std::lexicographical_compare(this->cbegin(), this->cend(), another.cbegin(), another.cend());
    }
};


namespace std
{
    inline std::string to_string(string_view sv)
    {
        return {sv.data(), sv.size()};
    }
}