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

TEST("char - ptrs")
{
    char array[] = {'a', 'c', 'b', 'd'};
    char array2[] = {'a', 'c', 'b', 'd'};

    EXPECT_CHAR_PTR(array, ==, array2);
    EXPECT_CHAR_PTR(array, !=, array + 1);

    EXPECT_CHAR_PTR(NULL, ==, NULL);
    EXPECT_CHAR_PTR(NULL, !=, array);

    EXPECT_CHAR_PTR(array + 2, <, array + 1);
    EXPECT_CHAR_PTR(array + 2, <=, array + 1);
    EXPECT_CHAR_PTR(array, <=, array2);
    EXPECT_CHAR_PTR(array + 1, >, array);
    EXPECT_CHAR_PTR(array + 1, >=, array);
    EXPECT_CHAR_PTR(array, >=, array);
}

TEST("char - strstr")
{
    EXPECT_CHAR('a', ==, *strchr("what a day", 'a'));
    EXPECT_CHAR('d', ==, *strchr("what a day", 'd'));
}
