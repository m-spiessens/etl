#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "etl/test.h"

#include "etl/map.h"

auto main() -> int
{
    {
        TEST("etl::Map constexpr");
        {
            constexpr etl::Map<uint8_t, uint8_t, 3> map = {{
                {5, 6},
                {1, 2},
                {3, 4},
            }};

            static_assert(map[1] == 2);
            static_assert(map[3] == 4);
            static_assert(map[5] == 6);

            EXPECT(map.size() == 3);
            EXPECT(map[1] == 2);
            EXPECT(map[3] == 4);
            EXPECT(map[5] == 6);
        }
    }
    {
        TEST("etl::Map");
        {
            etl::Map<uint8_t, uint8_t, 3> map = {{
                {1, 2},
                {3, 4},
                {5, 6}
            }};

            EXPECT(map.size() == 3);
            EXPECT(map[1] == 2);
            EXPECT(map[3] == 4);
            EXPECT(map[5] == 6);
        }
    }
}
