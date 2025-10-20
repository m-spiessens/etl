#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "etl/test.h"

#include "etl/buffer.h"

auto main() -> int
{
    {
        TEST("etl::Buffer");
        {
            std::array<uint16_t, 5> dataA = {1, 2, 3, 4, 5};
            etl::Buffer<uint16_t> bufferA(dataA);

            etl::Buffer<uint16_t> bufferB(bufferA);

            EXPECT(bufferA.data() == dataA.data());
            EXPECT(bufferA.size() == dataA.size());
            EXPECT(bufferA[0] == 1);
            EXPECT(bufferA == bufferB);
        }
        {
            std::array<uint16_t, 5> dataA = {1, 2, 3, 4, 5};
            etl::Buffer<uint16_t> bufferA(dataA.data(), dataA.size());

            std::array<uint16_t, 5> dataB = {1, 2, 3, 4, 5};
            etl::Buffer<uint16_t> bufferB(dataB);

            EXPECT(bufferA == bufferB);

            dataB[4] = 6;

            EXPECT(bufferA != bufferB);
            EXPECT(bufferA < bufferB);
            
        }
    }
}
