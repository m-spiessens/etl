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

#ifndef ETL_POOL_H_
#define ETL_POOL_H_

#include <stdint.h>

#include "queue.h"

namespace etl
{

/**
 * \brief A pool of DataType elements.
 *
 * A pool can be used to effectively pass big data structures through connections between components.
 * An element can be taken from the pool and passed around by reference.
 * When taking an element the new owner is responsible to give it back to the pool at some point.
 *
 * A pool is thread safe in the sense that the take() and release() can be called concurrently.
 */
template<typename DataType>
class Pool
{
private:
	size_t size;
	DataType* data;
	Queue<DataType*> available;

public:
	/**
	 * \brief Create a heap.
	 *
	 * The array of DataType will be allocated on the heap.
	 *
	 * \param size The size of the pool in number of DataType.
	 */
	explicit Pool(size_t size) :
			size(size),
			data(reinterpret_cast<DataType*>(malloc(size * sizeof(DataType)))),
			available(Queue<DataType*>(size))
	{
		for (size_t i = 0; i < size; i++)
		{
			available.enqueue(&data[i]);
		}
	}

	/**
	 * \brief Destructor.
	 *
	 * Deallocates the array of DataType from the heap.
	 */
	~Pool()
	{
		free(data);
	}

	/**
	 * \brief Is an element available in the pool?
	 */
	bool haveAvailable() const
	{
		return !available.empty();
	}

	/**
	 * \brief Take an element from the pool.
	 *
	 * When an element is taken from the pool, the new "owner" is responsible
	 * to release it back into the pool when it is no longer needed.
	 *
	 * \return Pointer to an element if the pool had one available.
	 * 		nullptr if no element was available.
	 */
	DataType* take()
	{
		DataType* take = nullptr;

		available.dequeue(take);

		return take;
	}

	/**
	 * \brief Release an element into the pool.
	 *
	 * \param element The element to be released into the pool.
	 * \return The element was successfully put in the pool.
	 * 		When not successful the take-release mechanism was violated.
	 */
	bool release(DataType& element)
	{
		return available.enqueue(&element);
	}
};

} // namespace etl

#endif // ETL_POOL_H_
