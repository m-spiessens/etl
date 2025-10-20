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

#ifndef ETL_LIST_H_
#define ETL_LIST_H_

#include <stddef.h>
#include <stdint.h>

// Embedded Template Library
namespace etl {

template<typename T>
class List
{
public:
    List(T* _element = nullptr);
    List(const List& other) = default;

    T* element() const;

    List& add(List& fragment);

    std::size_t size() const;

    List* next();

private:
    T* _element = nullptr;
    List* _next = nullptr;
};

template<typename T>
List<T>::List(T* _element) :
    _element(_element),
    _next(nullptr)
{
}

template<typename T>
T* List<T>::element() const
{
    return _element;
}

template<typename T>
List<T>& List<T>::add(List<T>& _element)
{
    if(this->_element == nullptr)
    {
        *this = _element;
    }
    else
    {
        List* tail = this;
        while(tail->_next != nullptr)
        {
            tail = tail->_next;
        }

        tail->_next = &_element;
    }
    return *this;
}

template<typename T>
List<T>* List<T>::next()
{
    return _next;
}

template<typename T>
std::size_t List<T>::size() const
{
    std::size_t size = 0;

    const List* chain = this;

    while(chain != nullptr)
    {
        size++;
        chain = chain->_next;
    }

    return size;
}

} // namespace My

#endif // ETL_LIST_H_
