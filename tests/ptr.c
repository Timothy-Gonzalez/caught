// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

static int array[] = {1, 2, 3, 4, 5};

TEST("ptr - basic")
{
    EXPECT_PTR(array, ==, array);
    EXPECT_PTR(array + 1, ==, array + 1);
    EXPECT_PTR(array + 3, !=, array + 2);
}

TEST("ptr - inequalities")
{
    EXPECT_PTR(array + 5, >, array + 2);
    EXPECT_PTR(array + 1, <, array + 3);
    EXPECT_PTR(array + 8, >=, array + 7);
    EXPECT_PTR(array + 8, >=, array + 8);
    EXPECT_PTR(array + 5, <=, array + 6);
    EXPECT_PTR(array + 7, <=, array + 7);
}

TEST("ptr - ptr")
{
    void *one = (void *)0x1;
    void *two = (void *)0x2;
    void *one_other = (void *)0x1;

    EXPECT_PTR_PTR(&one, ==, &one_other);
    EXPECT_PTR_PTR(&two, !=, &one);

    EXPECT_PTR_PTR(&two, >, &one);
    EXPECT_PTR_PTR(&one, <, &two);
    EXPECT_PTR_PTR(&two, >=, &one);
    EXPECT_PTR_PTR(&two, >=, &two);
    EXPECT_PTR_PTR(&one, <=, &two);
    EXPECT_PTR_PTR(&one, <=, &one);
}
