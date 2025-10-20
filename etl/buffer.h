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

#ifndef ETL_BUFFER_H_
#define ETL_BUFFER_H_

#include <assert.h>
#include <stddef.h>

#include <array>

// Embedded Template Library
namespace etl {

template<typename T>
class Buffer
{
public:
    Buffer(const T* data = nullptr, std::size_t size = 0) : _data(data), _size(size) {}
    
    template<std::size_t N>
    Buffer(const std::array<T, N>& data) : _data(data.data()), _size(data.size()) {}

    Buffer(const Buffer& other) = default;

    const T* data() const { return _data; }
    std::size_t size() const { return _size; }

    const T& operator[](std::size_t index) const
    {
        assert(index < _size);
        return _data[index];
    }

    bool operator==(const Buffer& other) const
    {
        if (_size != other._size) return false;
        for (std::size_t i = 0; i < _size; ++i)
        {
            if (_data[i] != other._data[i]) return false;
        }
        return true;
    }

    bool operator!=(const Buffer& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Buffer& other) const
    {
        for (std::size_t i = 0; i < _size && i < other._size; ++i)
        {
            if (_data[i] < other._data[i]) return true;
        }
        return (_size < other._size);
    }

private:
    const T* _data;
    std::size_t _size;
};

} // namespace etl

#endif // ETL_BUFFER_H_
