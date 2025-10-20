#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "etl/test.h"

#include "etl/string.h"

auto main() -> int
{
    {
        TEST("etl::String constexpr");
        {
            constexpr etl::String helloA("Hello");
            constexpr etl::String HelloW("HelloW");
            constexpr etl::String HelloB("Hello");

            static_assert(helloA[0] == 'H');
            static_assert(helloA.size() == 5);

            static_assert(HelloW.c_str()[5] == 'W');

            EXPECT(helloA.data() != nullptr);
            EXPECT(helloA.size() == 5);
            EXPECT(strcmp(helloA.c_str(), "Hello") == 0);

            EXPECT(HelloW.data() != nullptr);
            EXPECT(HelloW.size() == 6);
            EXPECT(strcmp(HelloW.c_str(), "HelloW") == 0);

            static_assert(helloA == HelloB);
            static_assert(helloA < HelloW);

            EXPECT(helloA == HelloB);
            EXPECT(helloA < HelloW);
        }
    }
    {
        TEST("etl::String");
        {
            etl::String helloA("Hello");
            etl::String HelloW("HelloW");
            etl::String HelloB("Hello");

            EXPECT(helloA.data() != nullptr);
            EXPECT(helloA.size() == 5);
            EXPECT(strcmp(helloA.c_str(), "Hello") == 0);

            EXPECT(HelloW.data() != nullptr);
            EXPECT(HelloW.size() == 6);
            EXPECT(strcmp(HelloW.c_str(), "HelloW") == 0);

            EXPECT(helloA == HelloB);
            EXPECT(helloA < HelloW);
            
            EXPECT(helloA[0] == 'H');
            EXPECT(HelloW.c_str()[5] == 'W');

        }
    }
    {
        TEST("etl::String with Buffer");
        {
            std::array<const unsigned char, 6> helloArray = {'H', 'e', 'l', 'l', 'o', '\0'};

            etl::Buffer<const unsigned char> buffer(helloArray);
            etl::String helloFromBuffer(buffer);

            EXPECT(helloFromBuffer.data() != nullptr);
            EXPECT(helloFromBuffer.size() == 6);
            EXPECT(strcmp(helloFromBuffer.c_str(), "Hello") == 0);
            
            EXPECT(helloFromBuffer[0] == 'H');
            EXPECT(helloFromBuffer.c_str()[1] == 'e');
        }
    }
}
