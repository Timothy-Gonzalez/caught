// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

#include <string.h>

TEST("char - basic")
{
    EXPECT_CHAR('a', ==, 'a');
    EXPECT_CHAR('a', !=, 'z');
}

TEST("char - inequalities")
{
    EXPECT_CHAR('B', <, 'c');
    EXPECT_CHAR('a', <=, 'z');
    EXPECT_CHAR('z', <=, 'z');
    EXPECT_CHAR('a', >, 'W');
    EXPECT_CHAR('c', >=, 'b');
    EXPECT_CHAR('c', >=, 'c');
}

TEST("char - strstr")
{
    EXPECT_CHAR('a', ==, *strchr("what a day", 'a'));
    EXPECT_CHAR('d', ==, *strchr("what a day", 'd'));
}
