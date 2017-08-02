#pragma once

#include <cstddef>
#include <type_traits>


template <class T>
class array_view
{
    using self = array_view;
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using iterator = pointer;
    using const_iterator = const_pointer;
    static constexpr size_type npos = size_type(-1);

public:
    array_view(pointer data, size_type size) noexcept :
            _data(data),
            _size(size)
    {
    }

    array_view() noexcept : array_view(nullptr, 0)
    {
    }

    size_type size() const noexcept
    {
        return _size;
    }

//    pointer data() noexcept
//    {
//        return _data;
//    }

    bool empty() const noexcept
    {
        return _data == nullptr || _size == 0;
    }

    pointer data() const noexcept
    {
        return _data;
    }

    T &operator[](const size_type index)
    {
        return _data[index];
    }

    const T &operator[](const size_type index) const
    {
        return _data[index];
    }

    T &front() noexcept
    {
        return _data[0];
    }

    const T &front() const noexcept
    {
        return _data[0];
    }

    T &back() noexcept
    {
        return _data[_size - 1];
    }

    const T &back() const noexcept
    {
        return _data[_size - 1];
    }

    bool operator==(const self &another) const noexcept
    {
        return _size == another._size && _data == another._data;
    }

    bool operator!=(const self &another) const noexcept
    {
        return !(*this == another);
    }

    self subview(size_type position, size_type count) const noexcept
    {
        return {_data + position, count};
    }

    size_type find(const_reference value, size_type position = 0) const noexcept
    {
        size_type result = npos;

        for (size_type i = position; i < size(); ++i)
        {
            if (_data[i] == value)
            {
                result = i;
                break;
            }
        }

        return result;
    }

    iterator begin() noexcept
    {
        return _data;
    }

    iterator end() noexcept
    {
        return _data + _size;
    }

    const_iterator cbegin() const noexcept
    {
        return _data;
    }

    const_iterator cend() const noexcept
    {
        return _data + _size;
    }

    size_type copy(typename std::remove_const<value_type>::type *dest, size_type count, size_type pos = 0) const
    {
        for (std::size_t i = pos; i < count; ++i)
        {
            dest[i] = _data[i];
        }
    }

protected:
    pointer _data;
    size_type _size;
};
