// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

#include <stdbool.h>

TEST("bool - basic")
{
    EXPECT_BOOL(true, ==, true);
    EXPECT_BOOL(false, ==, false);
    EXPECT_BOOL(true, !=, false);
    EXPECT_BOOL(false, !=, true);
}

TEST("bool - but bools are ints!")
{

    EXPECT_BOOL(true, >, false);
    EXPECT_BOOL(true, >=, false);
    EXPECT_BOOL(true, >=, true);

    EXPECT_BOOL(false, <, true);
    EXPECT_BOOL(false, <=, true);
    EXPECT_BOOL(false, <=, false);
}
