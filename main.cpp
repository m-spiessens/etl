#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "memorychain.h"

typedef etl::MemoryChain<uint8_t> MemoryChain;

auto main() -> int
{
    {
        MemoryChain chain;

        uint8_t _f1[] = { 1, 2, 3 };
        MemoryChain f1( _f1, sizeof(_f1) );

        uint8_t _f2[] = { 4 };
        MemoryChain f2( _f2, sizeof(_f2) );

        uint8_t _f3[] = { 5, 6 };
        MemoryChain f3( _f3, sizeof(_f3) );

        chain.add(f1).add(f2).add(f3);

        assert(chain.length() == 6);
    }
    
    {
        uint8_t _f1[] = { 1, 2, 3 };
        MemoryChain f1( _f1, sizeof(_f1) );

        uint8_t _f2[] = { 4 };
        MemoryChain f2( _f2, sizeof(_f2) );

        uint8_t _f3[] = { 5, 6 };
        MemoryChain f3( _f3, sizeof(_f3) );

        MemoryChain chain = f1.add(f2).add(f3);

        assert(chain.length() == 6);
    }

    {
        uint8_t _f1[] = { 1, 2, 3 };
        MemoryChain f1( _f1, sizeof(_f1) );

        uint8_t _f2[] = { 4 };
        MemoryChain f2( _f2, sizeof(_f2) );

        uint8_t _f3[] = { 5, 6 };
        MemoryChain f3( _f3, sizeof(_f3) );

        MemoryChain chain = f1.add(f2).add(f3);

        {
            uint8_t slice[2];
            size_t length = sizeof(slice);
            const void* s = chain.slice(slice, 0, length);
            assert(length == 2);

            uint8_t expected[] = { 1, 2 };
            assert(memcmp(s, expected, sizeof(expected)) == 0);
            assert(s == _f1);
        }

        {
            uint8_t slice[2];
            size_t length = sizeof(slice);
            const void* s = chain.slice(slice, 2, length);
            assert(length == 2);

            uint8_t expected[] = { 3, 4 };
            assert(memcmp(s, expected, sizeof(expected)) == 0);
        }
    }

    {
        uint8_t _f1[] = { 1, 2, 3 };
        MemoryChain f1( _f1, sizeof(_f1) );

        uint8_t _f2[] = { 4 };
        MemoryChain f2( _f2, sizeof(_f2) );

        uint8_t _f3[] = { 5, 6 };
        MemoryChain f3( _f3, sizeof(_f3) );

        MemoryChain chain = f1.add(f2).add(f3);

        {
            uint8_t slice[2];
            size_t length = sizeof(slice);
            const void* s = chain.slice(slice, 0, length);
            assert(length == 2);

            uint8_t expected[] = { 1, 2 };
            assert(memcmp(s, expected, sizeof(expected)) == 0);
            assert(s == _f1);
        }

        {
            uint8_t slice[2];
            size_t length = sizeof(slice);
            const void* s = chain.slice(slice, 2, length);
            assert(length == 2);

            uint8_t expected[] = { 3, 4 };
            assert(memcmp(s, expected, sizeof(expected)) == 0);
        }
    }

    {
        uint8_t _f1[] = { 1, 2, 3 };
        MemoryChain f1( _f1, sizeof(_f1) );

        uint8_t _f2[] = { 4 };
        MemoryChain f2( _f2, sizeof(_f2) );

        uint8_t _f3[] = { 5, 6 };
        MemoryChain f3( _f3, sizeof(_f3) );

        MemoryChain chain = f1.add(f2).add(f3);

        {
            uint8_t slice[2];
            size_t length = chain.take(slice, sizeof(slice));
            assert(length == 2);

            uint8_t expected[] = { 1, 2 };
            assert(memcmp(slice, expected, sizeof(expected)) == 0);
        }

        {
            uint8_t slice[2];
            size_t length = chain.take(slice, sizeof(slice));
            assert(length == 2);

            uint8_t expected[] = { 3, 4 };
            assert(memcmp(slice, expected, sizeof(expected)) == 0);
        }

        assert(chain.length() == 2);

        {
            uint8_t slice[2];
            size_t length = chain.take(slice, sizeof(slice));
            assert(length == 2);

            uint8_t expected[] = { 5, 6 };
            assert(memcmp(slice, expected, sizeof(expected)) == 0);
        }

        assert(chain.length() == 0);
    }

    {
        uint8_t _f1[] = { 1, 2, 3 };
        MemoryChain f1( _f1, sizeof(_f1) );

        uint8_t _f2[] = { 4 };
        MemoryChain f2( _f2, sizeof(_f2) );

        uint8_t _f3[] = { 5, 6 };
        MemoryChain f3( _f3, sizeof(_f3) );

        MemoryChain chain = f1.add(f2).add(f3);

        assert(chain.length() == 6);

        {
            const 
            uint8_t* slice;
            size_t length = chain.next(slice);
            assert(length == 3);

            assert(slice == _f1);
            uint8_t expected[] = { 1, 2, 3 };
            assert(memcmp(slice, expected, sizeof(expected)) == 0);
        }

        assert(chain.length() == 3);

        {
            const uint8_t* slice;
            size_t length = chain.next(slice);
            assert(length == 1);

            assert(slice == _f2);
            uint8_t expected[] = { 4 };
            assert(memcmp(slice, expected, sizeof(expected)) == 0);
        }

        assert(chain.length() == 2);

        {
            const uint8_t* slice;
            size_t length = chain.next(slice);
            assert(length == 2);

            assert(slice == _f3);
            uint8_t expected[] = { 5, 6 };
            assert(memcmp(slice, expected, sizeof(expected)) == 0);
        }

        assert(chain.length() == 0);
    }
}
