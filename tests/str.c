#include "../src/lib.h" // in your code, it would be #include "caught.h"
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
