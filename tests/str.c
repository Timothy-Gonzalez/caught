// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"
#include <string.h>

TEST("str - basic")
{
    EXPECT_STR("abc", ==, "abc");
    EXPECT_STR("def", ==, "def");
    EXPECT_STR(NULL, !=, "other");
    EXPECT_STR("one string", !=, "other");
}

TEST("str - strstr")
{
    char *str = "a long string that can be indexed";
    EXPECT_STR("be indexed", ==, strstr(str, "be"));
}
