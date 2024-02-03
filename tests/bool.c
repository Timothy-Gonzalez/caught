// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

#include <stdbool.h>

TEST("bool - basic")
{
    EXPECT_EQUAL_BOOL(true, true);
    EXPECT_EQUAL_BOOL(false, false);
}
