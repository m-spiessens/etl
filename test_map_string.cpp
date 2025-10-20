#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "etl/test.h"

#include "etl/string.h"
#include "etl/map.h"

auto main() -> int
{
    {
        TEST("etl::Map<etl::String>");
        {
            constexpr etl::Map<etl::String, uint8_t, 3> map = {{
                {etl::String("c"), 2},
                {etl::String("abc"), 4},
                {etl::String("ab"), 6}
            }};

            std::array<etl::String, 3> expectedKeys = 
            {
                etl::String("ab"),
                etl::String("abc"),
                etl::String("c")
            };

            EXPECT(map.keys() == expectedKeys);

            std::array<uint8_t, 3> expectedValues = {6, 4, 2};
            EXPECT(map.values() == expectedValues);

            EXPECT(map.size() == 3);
            EXPECT(map[etl::String("ab")] == 6);
            EXPECT(map[etl::String("abc")] == 4);
            EXPECT(map[etl::String("c")] == 2);
        }
    }
}
