#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "etl/test.h"

#include "etl/list.h"

auto main() -> int
{
    {
        TEST("etl::List");
        {
            uint16_t a = 42;
            etl::List<uint16_t> elementA(&a);

            uint16_t b = 43;
            etl::List<uint16_t> elementB(&b);

            uint16_t c = 41;
            etl::List<uint16_t> elementC(&c);

            elementA.add(elementB);
            elementA.add(elementC);

            EXPECT(elementA.element() == &a);
            EXPECT(elementA.next()->element() == &b);
            EXPECT(elementA.next()->next()->element() == &c);

            etl::List<uint16_t> sameList = elementA;
            
            EXPECT(sameList.size() == 3);
            EXPECT(sameList.element() == &a);
            EXPECT(sameList.next()->element() == &b);
            EXPECT(sameList.next()->next()->element() == &c);
        }
    }
}
