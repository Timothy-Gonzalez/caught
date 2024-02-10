// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

#include <stdbool.h>

TEST("expect - basic")
{
    EXPECT(true);
    EXPECT(1 + 1 == 2);

    EXPECT_NOT(false);
    EXPECT_NOT(6 * 9 == 42);
}
