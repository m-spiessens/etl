#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "queue.h"

typedef etl::Queue<uint8_t> Queue;

auto main() -> int
{
    {
        Queue queue(3);

        assert(queue.size() == 3);

        assert(queue.elements() == 0);

        queue.enqueue(9);

        assert(queue.size() == 3);

        assert(queue.elements() == 1);

        uint8_t element;
        bool success = queue.dequeue(element);
        assert(success);
        assert(element == 9);

        assert(queue.size() == 3);

        assert(queue.elements() == 0);
    }

    {
        Queue queue(3);

        assert(queue.size() == 3);

        assert(queue.elements() == 0);

        assert(queue.empty());
        assert(!queue.full());

        bool success = queue.enqueue(9);
        assert(success);

        assert(!queue.empty());
        assert(!queue.full());

        success = queue.enqueue(8);
        assert(success);

        assert(!queue.empty());
        assert(!queue.full());
        success = queue.enqueue(7);
        assert(success);

        assert(!queue.empty());
        assert(queue.full());

        success = queue.enqueue(6);
        assert(!success);

        assert(queue.elements() == 3);

        uint8_t element;
        success = queue.dequeue(element);
        assert(success);
        assert(element == 9);

        assert(queue.elements() == 2);

        success = queue.dequeue(element);
        assert(success);
        assert(element == 8);

        assert(queue.elements() == 1);

        success = queue.dequeue(element);
        assert(success);
        assert(element == 7);

        assert(queue.elements() == 0);

        success = queue.dequeue(element);
        assert(!success);

        assert(queue.elements() == 0);
    }

    {
        Queue queue(3);

        assert(queue.size() == 3);

        assert(queue.elements() == 0);

        for(size_t i = 0; i < UINT16_MAX; i++)
        {
            bool success = queue.enqueue(9);
            assert(success);

            uint8_t element;
            success = queue.dequeue(element);
            assert(success);
            assert(element == 9);
        }

        assert(queue.elements() == 0);

        bool success = queue.enqueue(8);
        assert(success);
        assert(queue.elements() == 1);

        success = queue.enqueue(7);
        assert(success);
        assert(queue.elements() == 2);
        
        success = queue.enqueue(6);
        assert(success);
        assert(queue.elements() == 3);
        
        success = queue.enqueue(5);
        assert(!success);
        assert(queue.elements() == 3);

        uint8_t element;
        success = queue.dequeue(element);
        assert(success);
        assert(element == 8);

        success = queue.dequeue(element);
        assert(success);
        assert(element == 7);

        success = queue.dequeue(element);
        assert(success);
        assert(element == 6);

        assert(queue.elements() == 0);

        success = queue.dequeue(element);
        assert(!success);

        assert(queue.elements() == 0);
    }

    {
        Queue queue(3);

        uint8_t element;
        bool success = queue.peek(element);
        assert(!success);

        queue.enqueue(1);
        assert(queue.elements() == 1);

        success = queue.peek(element);
        assert(success);
        assert(element == 1);
        assert(queue.elements() == 1);

        success = queue.dequeue(element);
        assert(success);
        assert(element == 1);
        assert(queue.elements() == 0);

        success = queue.peek(element);
        assert(!success);

        queue.enqueue(2);
        assert(queue.elements() == 1);

        success = queue.peek(element);
        assert(success);
        assert(element == 2);
        assert(queue.elements() == 1);
    }
}
