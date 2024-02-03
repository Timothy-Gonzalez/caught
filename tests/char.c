// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

#include <string.h>

TEST("char - basic")
{
    EXPECT_EQUAL_CHAR('a', 'a');
    EXPECT_EQUAL_CHAR('z', 'z');
}

TEST("char - basic")
{
    EXPECT_EQUAL_CHAR('a', *strchr("what a day", 'a'));
    EXPECT_EQUAL_CHAR('d', *strchr("what a day", 'd'));
}
