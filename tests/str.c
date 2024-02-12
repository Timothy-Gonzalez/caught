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

TEST("str - in")
{
    EXPECT_STR("abc", in, "abcdefghijk");
    EXPECT_STR("123", in, "12121232141246");

    EXPECT_STR("abc", not in, "bcdefghijk");
    EXPECT_STR("123", not in, "12121223141246");
}

TEST("str - match")
{
    EXPECT_STR("abc", match, "abc");
    EXPECT_STR("a a char", match, "a $c char");
    EXPECT_STR("a 123 number", match, "a $i number");
    EXPECT_STR("a 123.456 float", match, "a $f float");
    EXPECT_STR("the alphabet match", match, "the $a match");
    EXPECT_STR("the word123 match", match, "the $w match");
    EXPECT_STR("a very long string", match, "a $s string");
    EXPECT_STR("a literal $ can work", match, "a literal $$ can work");

    EXPECT_STR("a 123c number char", match, "a $i$c number char");
    EXPECT_STR("two nice looking - awesome beings strings", match, "two $s - $s strings");
    EXPECT_STR("will backtrace", match, "$s$s");

    EXPECT_STR("abc", not match, "def");
    EXPECT_STR("the char is missing: ", not match, "the char is missing: $c");
    EXPECT_STR("the no alpha 123 match", not match, "the no alpha $a match");
    EXPECT_STR("the no whitespace \t match", not match, "the no whitespace $w match");
    EXPECT_STR("the string is missing: ", not match, "the string is missing: $s");
    EXPECT_STR("not a match", not match, "not a $s match");

    // Evil test cases (not examples, stress testing verification only)
    EXPECT_STR("ABC_ABC_ABC", match, "$s_$s_$s");
    EXPECT_STR("ABC____ABC____ABC", match, "ABC$sABC$sABC");
    EXPECT_STR("WOW This is cool! WOW This is cool! WOW This is cool! WOW This is cool!",
               match, "WOW This is cool!$sWOW This is cool!$sWOW This is cool!$sWOW This is cool!");
    EXPECT_STR("WOW This is cool! WOW This is cool! WOW This is cool! WOW This is cool",
               not match, "WOW This is cool!$sWOW This is cool!$sWOW This is cool!$sWOW This is cool!");
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

TEST("str - array in")
{
    char *array[] = {"123", "456", "789"};

    EXPECT_STR_ARRAY_ELEMENT("123", in, array, 3);
    EXPECT_STR_ARRAY_ELEMENT("456", in, array, 3);

    EXPECT_STR_ARRAY_ELEMENT("346", not in, array, 3);
    EXPECT_STR_ARRAY_ELEMENT("789", not in, array, 2);

    char *null_terminated[] = {"abc", "def", NULL};

    EXPECT_STR_ARRAY_ELEMENT("abc", in, null_terminated, -1);
    EXPECT_STR_ARRAY_ELEMENT("wxy", not in, null_terminated, -1);
}
