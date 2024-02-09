// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"
#include <string.h>

TEST("str - strstr")
{
    char *str = "a long string that can be indexed";
    EXPECT_STR("be indexed", ==, strstr(str, "be"));
}

TEST("str - basic")
{
    EXPECT_STR("abc", ==, "abc");
    EXPECT_STR("def", ==, "def");
    EXPECT_STR(NULL, !=, "other");
    EXPECT_STR("one string", !=, "other");
}

TEST("str - ptrs")
{
    char *array[] = {"abc", "def", NULL};
    char *array2[] = {"abc", "def", NULL};

    EXPECT_STR_PTR(array, ==, array2);
    EXPECT_STR_PTR(array, !=, array + 1);

    EXPECT_STR_PTR(NULL, ==, NULL);
    EXPECT_STR_PTR(NULL, !=, array);
    EXPECT_STR_PTR(array, !=, NULL);
    EXPECT_STR_PTR(NULL, !=, array + 2);
}

TEST("str - arrays")
{
    char *null_terminated[] = {"abc", "def", NULL};
    char *null_terminated2[] = {"abc", "def", NULL};
    char *array[] = {"123", "456", "789"};
    char *array2[] = {"123", "456", "789"};

    EXPECT_STR_ARRAY(null_terminated, ==, null_terminated2, -1);
    null_terminated[1] = "fed";
    EXPECT_STR_ARRAY(null_terminated, !=, null_terminated2, -1);

    EXPECT_STR_ARRAY(array, ==, array2, 3);
    array[2] = "000";
    EXPECT_STR_ARRAY(array, !=, array2, 3);
    EXPECT_STR_ARRAY(array, ==, array2, 2);
}
