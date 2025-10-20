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

#ifndef ETL_QUEUE_H_
#define ETL_QUEUE_H_

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include <atomic>

namespace etl
{

namespace doNotUse
{

class GenericQueue
{
private:
	const std::size_t elementCount;
	const std::size_t elementSize;
	uint8_t* data;
	std::atomic<uint16_t> first{ 0 };
	std::atomic<uint16_t> last{ 0 };
	std::atomic<uint16_t> enqueued{ 0 };
	std::atomic<uint16_t> dequeued{ 0 };

protected:
	/**
	 * \brief Create a queue.
	 *
	 * The array of DataType will be allocated on the heap.
	 *
	 * \param size The size of the queue in number of DataType.
	 */
	explicit GenericQueue(std::size_t elementCount, std::size_t elementSize) :
			elementCount(elementCount),
			elementSize(elementSize)
	{
		assert(elementCount < UINT16_MAX);
		data = reinterpret_cast<uint8_t*>(malloc(elementCount * elementSize));
	}

	/**
	 * \brief Destructor.
	 *
	 * Deallocates the array of DataType from the heap.
	 */
	~GenericQueue()
	{
		free(data);
	}

	/**
	 * \brief Enqueue an element of DataType.
	 *
	 * Can be called concurrently with respect to dequeue().
	 * If the queue is full the given element is not added.
	 *
	 * \param element The element to be enqueued.
	 * \return The element was successfully enqueued.
	 */
	bool enqueue(const void* element)
	{
		bool success = !full();

		if (success)
		{
			memcpy(&data[last * elementSize], element, elementSize);

			last = (last == size() - 1) ? 0 : last + 1;

			enqueued++;
		}

		return success;
	}

	/**
	 * \brief Dequeue an element of DataType.
	 *
	 * Can be called concurrently with respect to enqueue().
	 *
	 * \param element [output] The dequeued element.
	 * 		The return value indicates whether the element is valid.
	 * \return An element was successfully dequeued.
	 * 		Thus the element output parameter has a valid value.
	 */
	bool dequeue(void* element)
	{
		bool success = !empty();

		if (success)
		{
			memcpy(element , &data[first * elementSize], elementSize);

			first = (first == size() - 1) ? 0 : first + 1;

			dequeued++;
		}

		return success;
	}

	/**
	 * \brief Peek in the queue.
	 *
	 * Does not modify the queue in any way.
	 *
	 * \param element [output] The next element to be dequeued.
	 * 		The return value indicates whether the element is valid.
	 * \return The queue is not empty.
	 * 		Thus the element output parameter has a valid value.
	 */
	bool peek(void* element) const
	{
		bool success = !empty();

		if (success)
		{
			memcpy(element , &data[first * elementSize], elementSize);

			success = true;
		}

		return success;
	}

public:
	bool empty() const
	{
		return (enqueued == dequeued);
	}

	bool full() const
	{
		return (enqueued == static_cast<uint16_t>(dequeued + size()));
	}

	bool peek() const
	{
		return !empty();
	}

	std::size_t elements() const
	{
		int32_t delta = static_cast<int32_t>(enqueued) - static_cast<int32_t>(dequeued);

		return static_cast<std::size_t>((delta >= 0) ? delta : delta + UINT16_MAX + 1);
	}

	std::size_t size() const
	{
		return elementCount;
	}
};

} // namespace doNotUse

template<typename Type>
class Queue : 
		public doNotUse::GenericQueue
{
public:
	explicit Queue(std::size_t size) :
			doNotUse::GenericQueue(size, sizeof(Type))
	{
	}

	/**
	 * \brief Enqueue an element of DataType.
	 *
	 * Can be called concurrently with respect to dequeue().
	 * If the queue is full the given element is not added.
	 *
	 * \param element The element to be enqueued.
	 * \return The element was successfully enqueued.
	 */
	bool enqueue(const Type& element)
	{
		return doNotUse::GenericQueue::enqueue(&element);
	}
	/**
	 * \brief Dequeue an element of DataType.
	 *
	 * Can be called concurrently with respect to enqueue().
	 *
	 * \param element [output] The dequeued element.
	 * 		The return value indicates whether the element is valid.
	 * \return An element was successfully dequeued.
	 * 		Thus the element output parameter has a valid value.
	 */
	bool dequeue(Type& element)
	{
		return doNotUse::GenericQueue::dequeue(&element);
	}
	
	/**
	 * \brief Peek in the queue.
	 *
	 * Does not modify the queue in any way.
	 *
	 * \param element [output] The next element to be dequeued.
	 * 		The return value indicates whether the element is valid.
	 * \return The queue is not empty.
	 * 		Thus the element output parameter has a valid value.
	 */
	bool peek(Type& element) const
	{
		return doNotUse::GenericQueue::peek(&element);
	}
};

template<>
class Queue<void>
{
private:
	std::atomic<uint16_t> enqueued = 0;
	std::atomic<uint16_t> dequeued = 0;
	const std::size_t elementCount;

public:
	explicit Queue(std::size_t size) :
			elementCount(size)
	{
	}

	bool empty() const
	{
		return (enqueued == dequeued);
	}

	bool full() const
	{
		return (enqueued == static_cast<uint16_t>(dequeued + size()));
	}

	bool peek() const
	{
		return !empty();
	}

	std::size_t elements() const
	{
		int32_t delta = static_cast<int32_t>(enqueued) - static_cast<int32_t>(dequeued);

		return static_cast<std::size_t>((delta >= 0) ? delta : delta + UINT16_MAX + 1);
	}

	std::size_t size() const
	{
		return elementCount;
	}

	/**
	 * \brief Enqueue.
	 *
	 * Can be called concurrently with respect to dequeue().
	 * If the queue is full the given element is not added.
	 *
	 * \return True
	 */
	bool enqueue()
	{
		bool success = !full();

		if(success)
		{
			enqueued++;
		}

		return success;
	}

	/**
	 * \brief Dequeue an element of DataType.
	 *
	 * Can be called concurrently with respect to enqueue().
	 *
	 * \param element [output] The dequeued element.
	 * 		The return value indicates whether the element is valid.
	 * \return An element was successfully dequeued.
	 * 		Thus the element output parameter has a valid value.
	 */
	bool dequeue()
	{
		bool success = peek();

		if(success)
		{
			dequeued++;
		}

		return success;
	}
};

} // namespace etl

#endif // ETL_QUEUE_H_
