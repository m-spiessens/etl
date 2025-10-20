/* The MIT License (MIT)
 *
 * Copyright (c) 2025 Mathias Spiessens
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software, hardware and associated documentation files (the "Solution"), to deal
 * in the Solution without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Solution, and to permit persons to whom the Solution is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Solution.
 *
 * THE SOLUTION IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOLUTION OR THE USE OR OTHER DEALINGS IN THE
 * SOLUTION.
 */

#ifndef ETL_STRING_H_
#define ETL_STRING_H_

#include <stddef.h>

#include "etl/buffer.h"

// Embedded Template Library
namespace etl {

class String
{
public:
    template<std::size_t N>
    constexpr String(const char (&str)[N]);

    constexpr String();

    String(const Buffer<const unsigned char>& buffer);

    String(const String& other) = default;

    const unsigned char* data() const;
    constexpr std::size_t size() const;

    constexpr const char* c_str()const ;
    constexpr const char& operator[](std::size_t index) const;

    constexpr bool operator==(const String& other) const;
    constexpr bool operator<(const String& other) const;

private:
    const char* _data;
    std::size_t _size;
};

template<std::size_t N>
constexpr String::String(const char (&str)[N])
    : _data(str), _size(N - 1) {}

constexpr String::String()
    : _data(nullptr), _size(0u) {}

constexpr std::size_t String::size() const { return _size; }

constexpr bool String::operator<(const String& other) const
{
    for (std::size_t i = 0; i < _size && i < other._size; ++i)
    {
        if (_data[i] < other._data[i]) return true;
    }
    return (_size < other._size);
}

constexpr const char* String::c_str() const { return _data; }
constexpr const char& String::operator[](std::size_t index) const
{
    assert(index < _size);
    return _data[index];
}

constexpr bool String::operator==(const String& other) const
{
    if (_size != other._size) return false;
    for (std::size_t i = 0; i < _size; ++i)
    {
        if (_data[i] != other._data[i]) return false;
    }
    return true;
}


} // namespace etl

#endif // ETL_STRING_H_
