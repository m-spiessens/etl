#include <stddef.h>
#include <string.h>

#include "etl/test.h"

#include "etl/memorychain.h"

typedef etl::MemoryChain<uint8_t> MemoryChain;

auto main() -> int
{
    {
        TEST("etl::MemoryChain");
        {
            MemoryChain chain;

            uint8_t _f1[] = { 1, 2, 3 };
            MemoryChain f1( _f1, sizeof(_f1) );

            uint8_t _f2[] = { 4 };
            MemoryChain f2( _f2, sizeof(_f2) );

            uint8_t _f3[] = { 5, 6 };
            MemoryChain f3( _f3, sizeof(_f3) );

            chain.add(f1).add(f2).add(f3);

            EXPECT(chain.length() == 6);
        }
        
        {
            uint8_t _f1[] = { 1, 2, 3 };
            MemoryChain f1( _f1, sizeof(_f1) );

            uint8_t _f2[] = { 4 };
            MemoryChain f2( _f2, sizeof(_f2) );

            uint8_t _f3[] = { 5, 6 };
            MemoryChain f3( _f3, sizeof(_f3) );

            MemoryChain chain = f1.add(f2).add(f3);

            EXPECT(chain.length() == 6);
        }

        {
            MemoryChain chain;

            {
                uint8_t slice[2];
                std::size_t length = sizeof(slice);
                auto s = chain.slice(slice, length);
                EXPECT(s.size() == 0);
                EXPECT(s.data() == &slice[0]);
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
                std::array<uint8_t, 2> slice = { 0, 0 };
                auto s = chain.slice(slice.data(), slice.size());
                EXPECT(s.size() == 2);

                std::array<uint8_t, 2> expected = { 1, 2 };
                EXPECT(memcmp(s.data(), expected.data(), expected.size()) == 0);
                EXPECT(slice != expected);
                EXPECT(s.data() == _f1);
            }

            {
                std::array<uint8_t, 2> slice = { 0, 0 };
                auto s = chain.slice(slice.data(), slice.size(), 2);
                EXPECT(s.size() == 2);

                std::array<uint8_t, 2> expected = { 3, 4 };
                EXPECT(memcmp(s.data(), expected.data(), expected.size()) == 0);
                EXPECT(slice == expected);
                EXPECT(s.data() == slice.data());
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
                std::array<uint8_t, 6> slice = { 0 };
                auto s = chain.slice(slice.data(), slice.size());
                EXPECT(s.size() == 6);

                std::array<uint8_t, 6> expected = { 1, 2, 3, 4, 5, 6 };
                EXPECT(memcmp(s.data(), expected.data(), expected.size()) == 0);
                EXPECT(slice == expected);
                EXPECT(s.data() == slice.data());
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
                std::size_t length = chain.take(slice, sizeof(slice));
                EXPECT(length == 2);

                uint8_t expected[] = { 1, 2 };
                EXPECT(memcmp(slice, expected, sizeof(expected)) == 0);
            }

            {
                uint8_t slice[2];
                std::size_t length = chain.take(slice, sizeof(slice));
                EXPECT(length == 2);

                uint8_t expected[] = { 3, 4 };
                EXPECT(memcmp(slice, expected, sizeof(expected)) == 0);
            }

            EXPECT(chain.length() == 2);

            {
                uint8_t slice[2];
                std::size_t length = chain.take(slice, sizeof(slice));
                EXPECT(length == 2);

                uint8_t expected[] = { 5, 6 };
                EXPECT(memcmp(slice, expected, sizeof(expected)) == 0);
            }

            EXPECT(chain.length() == 0);
        }

        {
            uint8_t _f1[] = { 1, 2, 3 };
            MemoryChain f1( _f1, sizeof(_f1) );

            uint8_t _f2[] = { 4 };
            MemoryChain f2( _f2, sizeof(_f2) );

            uint8_t _f3[] = { 5, 6 };
            MemoryChain f3( _f3, sizeof(_f3) );

            MemoryChain chain = f1.add(f2).add(f3);

            EXPECT(chain.length() == 6);

            {
                const uint8_t* slice;
                std::size_t length = chain.next(slice);
                EXPECT(length == 3);

                EXPECT(slice == _f1);
                uint8_t expected[] = { 1, 2, 3 };
                EXPECT(memcmp(slice, expected, sizeof(expected)) == 0);
            }

            EXPECT(chain.length() == 3);

            {
                const uint8_t* slice;
                std::size_t length = chain.next(slice);
                EXPECT(length == 1);

                EXPECT(slice == _f2);
                uint8_t expected[] = { 4 };
                EXPECT(memcmp(slice, expected, sizeof(expected)) == 0);
            }

            EXPECT(chain.length() == 2);

            {
                const uint8_t* slice;
                std::size_t length = chain.next(slice);
                EXPECT(length == 2);

                EXPECT(slice == _f3);
                uint8_t expected[] = { 5, 6 };
                EXPECT(memcmp(slice, expected, sizeof(expected)) == 0);
            }

            EXPECT(chain.length() == 0);
        }
    }
    
    {
        TEST("etl::MemoryChain iterator");
        {
            uint8_t _f1[] = { 1, 2, 3 };
            MemoryChain chain( _f1, sizeof(_f1) );

            uint8_t _f2[] = { 4 };
            MemoryChain f2( _f2, sizeof(_f2) );

            uint8_t _f3[] = { 5, 6 };
            MemoryChain f3( _f3, sizeof(_f3) );

            f2.add(f3);
            chain.add(f2);

            EXPECT(chain.length() == 6);

            unsigned int i = 1;
            for(auto& byte : chain)
            {
                EXPECT(byte == i);
                ++i;
            }
        }
    }
}
