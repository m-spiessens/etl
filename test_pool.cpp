#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "pool.h"

typedef etl::Pool<uint8_t> Pool;

auto main() -> int
{
    {
        Pool pool(2);

        assert(pool.haveAvailable());

        uint8_t* element1 = nullptr;
        element1 = pool.take();
        assert(element1 != nullptr);

        assert(pool.haveAvailable());

        uint8_t* element2 = nullptr;
        element2 = pool.take();
        assert(element2 != nullptr);

        assert(!pool.haveAvailable());

        uint8_t* element3 = nullptr;
        element3 = pool.take();
        assert(element3 == nullptr);

        assert(!pool.haveAvailable());

        bool success = pool.release(*element1);
        assert(success);

        assert(pool.haveAvailable());

        success = pool.release(*element2);
        assert(success);

        assert(pool.haveAvailable());

        success = pool.release(*element2);
        assert(!success);
    }
}
