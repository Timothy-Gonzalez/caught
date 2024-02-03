// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"
#include <string.h>

TEST("str - basic")
{
    EXPECT_EQUAL_STR("abc", "abc");
    EXPECT_EQUAL_STR("def", "def");
}

TEST("str - strstr")
{
    char *str = "a long string that can be indexed";
    EXPECT_EQUAL_STR("be indexed", strstr(str, "be"));
}
