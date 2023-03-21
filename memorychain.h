/* The MIT License (MIT)
 *
 * Copyright (c) 2021 Mathias Spiessens
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

#ifndef MEMORYCHAIN_H_
#define MEMORYCHAIN_H_

#include <stdint.h>

// Embedded Template Library
namespace etl {

template<typename T>
class MemoryChain
{
public:
    MemoryChain(const T* data = nullptr, size_t length = 0);

    MemoryChain& add(MemoryChain& fragment);

    const T* slice(T* slice, size_t offset, size_t& length) const;

    size_t take(T* taken, size_t length);

    size_t next(const T*& fragment);

    size_t length() const;

private:
    MemoryChain* _next = nullptr;

    struct {
        const T* data;
        size_t length;
    } fragment = 
        { 
            .data = nullptr, 
            .length = 0 
        };

    void takeSome(T* taken, size_t& length);

public:

    friend class Iterator;
};

template<typename T>
MemoryChain<T>::MemoryChain(const T* data, size_t length) :
    _next(nullptr),
    fragment({ .data = data, .length = length})
{
}

template<typename T>
MemoryChain<T>& MemoryChain<T>::add(MemoryChain<T>& fragment)
{
    if(this->fragment.data == nullptr)
    {
        *this = fragment;
    }
    else
    {
        MemoryChain* tail = this;
        while(tail->_next != nullptr)
        {
            tail = tail->_next;
        }

        tail->_next = &fragment;
    }

    return *this;
}

template<typename T>
const T* MemoryChain<T>::slice(T* slice, size_t offset, size_t& length) const
{
    assert(slice != nullptr);

    if(length == 0)
    {
        return 0;
    }

    size_t requested = length;

    const MemoryChain* chain = this;

    while(chain != nullptr && chain->fragment.length <= offset)
    {
        offset -= chain->fragment.length;
        chain = chain->_next;
    }

    // Provide in-place slice if possible:
    if(chain != NULL && chain->fragment.length >= offset + requested)
    {
        return &chain->fragment.data[offset];
    }

    uint8_t* slice_cursor = slice;
    while(chain != NULL && length > 0)
    {
        size_t copy = (length < chain->fragment.length - offset) ? length : chain->fragment.length - offset;
        memcpy(slice_cursor, &chain->fragment.data[offset], copy);
        length -= copy;
        slice_cursor += copy;

        offset = 0;

        chain = chain->_next;
    }

    length = requested - length;

    return slice;
}

template<typename T>
void MemoryChain<T>::takeSome(T* taken, size_t& length)
{
    if(fragment.length <= length) // Consume fragment:
    {
        memcpy(taken, fragment.data, fragment.length);
        taken += fragment.length;
        length -= fragment.length;
        fragment.length = 0;

        if(_next != nullptr && length >= 0)
        {
            *this = *_next;
            takeSome(taken, length);
        }
    }
    else
    {
        memcpy(taken, fragment.data, length);
        fragment.data = fragment.data + length;
        fragment.length -= length;
        length = 0;
    }
}

template<typename T>
size_t MemoryChain<T>::take(T* taken, size_t length)
{
    assert(taken != NULL);

    size_t took = length;

    takeSome(taken, length);

    return took - length;
}

template<typename T>
size_t MemoryChain<T>::next(const T*& fragment)
{
    fragment = this->fragment.data;
    size_t length = this->fragment.length;

    if(_next != nullptr)
    {
        *this = *_next;
    }
    else
    {
        this->fragment.data = nullptr;
        this->fragment.length = 0;
    }

    return length;
}

template<typename T>
size_t MemoryChain<T>::length() const
{
    size_t length = 0;

    const MemoryChain* chain = this;

    while(chain != nullptr)
    {
        length += chain->fragment.length;
        chain = chain->_next;
    }

    return length;
}

} // namespace My

#endif /* MEMORYCHAIN_H_ */
