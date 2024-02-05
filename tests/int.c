// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("int - simple math")
{
    EXPECT_INT(2, ==, 1 + 1);
    EXPECT_INT(7, ==, 3 + 4);
    EXPECT_INT(42, !=, 6 * 9);
}

TEST("int - inequalities")
{
    EXPECT_INT(734, >, 5);
    EXPECT_INT(-411, <, -1);
    EXPECT_INT(6, >=, 5);
    EXPECT_INT(5, >=, 5);
    EXPECT_INT(2, <=, 3);
    EXPECT_INT(3, <=, 3);
}

TEST("int - ptrs")
{
    int array[] = {1, 5, 3, 2};
    int array2[] = {1, 5, 3, 2};

    EXPECT_INT_PTR(array, ==, array2);
    EXPECT_INT_PTR(array, !=, array + 1);

    EXPECT_INT_PTR(NULL, !=, array);
    EXPECT_INT_PTR(array, !=, NULL);

    EXPECT_INT_PTR(array + 1, >, array);
    EXPECT_INT_PTR(array + 2, <, array + 1);
    EXPECT_INT_PTR(array + 1, >=, array);
    EXPECT_INT_PTR(array, >=, array2);
    EXPECT_INT_PTR(array + 2, <=, array + 1);
    EXPECT_INT_PTR(array, <=, array);
}

int factorial(n)
{
    if (n <= 1)
    {
        return 1;
    }
    return n * factorial(n - 1);
}

TEST("int - factorial")
{
    EXPECT_INT(1, ==, factorial(1));
    EXPECT_INT(2, ==, factorial(2));
    EXPECT_INT(6, ==, factorial(3));
    EXPECT_INT(24, ==, factorial(4));
    EXPECT_INT(120, ==, factorial(5));
    EXPECT_INT(720, ==, factorial(6));
}
