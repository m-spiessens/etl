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

#ifndef ETL_MEMORYCHAIN_H_
#define ETL_MEMORYCHAIN_H_

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "buffer.h"

// Embedded Template Library
namespace etl {

template<typename T>
class MemoryChain
{
public:
    MemoryChain(const T* data = nullptr, std::size_t length = 0);
    MemoryChain(Buffer<T> fragment);
    MemoryChain(const MemoryChain& other) = default;

    MemoryChain& add(MemoryChain& _fragment);

    Buffer<const T> slice(T* slice, std::size_t length, std::size_t offset = 0) const;

    std::size_t take(T* taken, std::size_t length);

    std::size_t drop(std::size_t length);

    std::size_t next(const T*& _fragment);

    std::size_t length() const;

    MemoryChain* next();

    const T* fragment(std::size_t& length) const;

    class iterator {
    public:
        iterator(MemoryChain* chain, std::size_t offset = 0)
            : chain(chain), offset(offset) {
            advanceToValid();
        }
        iterator& operator++() { // prefix
            if (!chain) return *this;
            ++offset;
            if (offset >= chain->_fragment.size()) {
                chain = chain->_next;
                offset = 0;
                advanceToValid();
            }
            return *this;
        }
        iterator operator++(int) { // postfix
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator!=(const iterator& other) const {
            return chain != other.chain || offset != other.offset;
        }
        T& operator*() const {
            return const_cast<T&>(chain->_fragment.data()[offset]);
        }
    private:
        MemoryChain* chain;
        std::size_t offset;
        void advanceToValid() {
            while (chain && chain->_fragment.size() == 0) {
                chain = chain->_next;
                offset = 0;
            }
        }
    };
    iterator begin() { return iterator(this); }
    iterator end() { return iterator(nullptr); }

    class const_iterator {
    public:
        const_iterator(const MemoryChain* chain, std::size_t offset = 0)
            : chain(chain), offset(offset) {
            advanceToValid();
        }
        const_iterator& operator++() {
            if (!chain) return *this;
            ++offset;
            if (offset >= chain->_fragment.size()) {
                chain = chain->_next;
                offset = 0;
                advanceToValid();
            }
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator!=(const const_iterator& other) const {
            return chain != other.chain || offset != other.offset;
        }
        const T& operator*() const {
            return chain->_fragment.data()[offset];
        }
    private:
        const MemoryChain* chain;
        std::size_t offset;
        void advanceToValid() {
            while (chain && chain->_fragment.size() == 0) {
                chain = chain->_next;
                offset = 0;
            }
        }
    };
    const_iterator begin() const { return const_iterator(this); }
    const_iterator end() const { return const_iterator(nullptr); }

private:
    MemoryChain* _next = nullptr;

    Buffer<T> _fragment;

    void takeSome(T* taken, std::size_t& length);
};

template<typename T>
MemoryChain<T>::MemoryChain(const T* data, std::size_t length) :
    _next(nullptr),
    _fragment{ data, length}
{}

template<typename T>
MemoryChain<T>::MemoryChain(Buffer<T> fragment) : 
    _next(nullptr), 
    _fragment(fragment) 
{}

template<typename T>
MemoryChain<T>& MemoryChain<T>::add(MemoryChain<T>& _fragment)
{
    if(this->_fragment.data() == nullptr)
    {
        *this = _fragment;
    }
    else
    {
        MemoryChain* tail = this;
        while(tail->_next != nullptr)
        {
            tail = tail->_next;
        }

        tail->_next = &_fragment;
    }
    return *this;
}

template<typename T>
Buffer<const T> MemoryChain<T>::slice(T* slice, std::size_t length, std::size_t offset) const
{
    assert(slice != nullptr);

    if(length == 0)
    {
        return nullptr;
    }

    std::size_t requested = length;

    const MemoryChain* chain = this;

    while(chain != nullptr && chain->_fragment.size() <= offset)
    {
        offset -= chain->_fragment.size();
        chain = chain->_next;
    }

    // Provide in-place slice if possible:
    if(chain != nullptr && chain->_fragment.size() >= offset + requested)
    {
        return Buffer<const T>(&chain->_fragment.data()[offset], requested);
    }

    uint8_t* slice_cursor = slice;
    while(chain != nullptr && length > 0)
    {
        std::size_t copy = (length < chain->_fragment.size() - offset) ? length : chain->_fragment.size() - offset;
        memcpy(slice_cursor, &chain->_fragment.data()[offset], copy);
        length -= copy;
        slice_cursor += copy;

        offset = 0;

        chain = chain->_next;
    }

    return Buffer<const T>(slice, requested - length);
}

template<typename T>
void MemoryChain<T>::takeSome(T* taken, std::size_t& length)
{
    if(_fragment.size() <= length) // Consume _fragment:
    {
        memcpy(taken, _fragment.data(), _fragment.size());
        taken += _fragment.size();
        length -= _fragment.size();

        if(_next != nullptr)
        {
            *this = *_next;
            if(length > 0)
            {
                takeSome(taken, length);
            }
        }
        else
        {
            this->_fragment = Buffer<T>();
        }
    }
    else
    {
        memcpy(taken, _fragment.data(), length);
        _fragment = Buffer<T>(_fragment.data() + length, _fragment.size() - length);
        length = 0;
    }
}

template<typename T>
std::size_t MemoryChain<T>::take(T* taken, std::size_t length)
{
    assert(taken != NULL);

    std::size_t took = length;

    takeSome(taken, length);

    return took - length;
}

template<typename T>
std::size_t MemoryChain<T>::drop(std::size_t length)
{
    std::size_t took = length;

    if(_fragment.size() <= length) // Consume _fragment:
    {
        length -= _fragment.size();

        if(_next != nullptr)
        {
            *this = *_next;
            if(length > 0)
            {
                drop(length);
            }
        }
        else
        {
            this->_fragment = Buffer<T>();
        }
    }
    else
    {
        _fragment = Buffer<T>(_fragment.data() + length, _fragment.size() - length);
        length = 0;
    }

    return took - length;
}

template<typename T>
std::size_t MemoryChain<T>::next(const T*& _fragment)
{
    _fragment = this->_fragment.data();
    std::size_t length = this->_fragment.size();

    if(_next != nullptr)
    {
        *this = *_next;
    }
    else
    {
        this->_fragment = Buffer<T>();
    }

    return length;
}

template<typename T>
MemoryChain<T>* MemoryChain<T>::next()
{
    return _next;
}

template<typename T>
const T* MemoryChain<T>::fragment(std::size_t& length) const
{
    length = _fragment.size();
    return _fragment.data();
}

template<typename T>
std::size_t MemoryChain<T>::length() const
{
    std::size_t length = 0;

    const MemoryChain* chain = this;

    while(chain != nullptr)
    {
        length += chain->_fragment.size();
        chain = chain->_next;
    }

    return length;
}

} // namespace My

#endif // ETL_MEMORYCHAIN_H_
