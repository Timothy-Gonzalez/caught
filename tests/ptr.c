// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("ptr - basic")
{
    int array[] = {1, 2, 3, 4, 5};
    EXPECT_EQUAL_PTR(array, array);
    EXPECT_EQUAL_PTR(array + 1, array + 1);
    EXPECT_EQUAL_PTR(array + 3, array + 3);
}
