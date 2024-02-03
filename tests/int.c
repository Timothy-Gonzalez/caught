// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("int - 1 + 1 = 2")
{
    EXPECT_EQUAL_INT(2, 1 + 1);
}

TEST("int - 3 + 4 = 7")
{
    EXPECT_EQUAL_INT(7, 3 + 4);
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
    EXPECT_EQUAL_INT(1, factorial(1));
    EXPECT_EQUAL_INT(2, factorial(2));
    EXPECT_EQUAL_INT(6, factorial(3));
    EXPECT_EQUAL_INT(24, factorial(4));
    EXPECT_EQUAL_INT(120, factorial(5));
    EXPECT_EQUAL_INT(720, factorial(6));
}
