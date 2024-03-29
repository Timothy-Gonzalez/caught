// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

#include <string.h>

TEST("char - strstr")
{
    EXPECT_CHAR('a', ==, *strchr("what a day", 'a'));
    EXPECT_CHAR('d', ==, *strchr("what a day", 'd'));
}

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

TEST("char - arrays")
{
    char array[] = {'a', 'b', 'c'};
    char array2[] = {'a', 'b', 'c'};
    char array3[] = {'b', 'c', 'd'};

    EXPECT_CHAR_ARRAY(array, ==, array2, 3);
    EXPECT_CHAR_ARRAY(array, >=, array2, 3);
    EXPECT_CHAR_ARRAY(array, <=, array2, 3);
    array[2] = 'w';
    EXPECT_CHAR_ARRAY(array, !=, array2, 3);
    EXPECT_CHAR_ARRAY(array, ==, array2, 2);

    EXPECT_CHAR_ARRAY(array2, <, array3, 3);
    EXPECT_CHAR_ARRAY(array3, >, array2, 3);

    // Can also use c-strings, though recommended to just use EXPECT_STR methods instead
    // only benefit of this is getting explicit output of each character
    char string[] = "xyz";
    char string2[] = "xyz";
    EXPECT_CHAR_ARRAY(string, ==, string2, -1);
    string[2] = 'w';
    EXPECT_CHAR_ARRAY(string, !=, string2, -1);
}

TEST("char - in")
{
    char array[] = {'a', 'b', 'c'};

    EXPECT_CHAR_ARRAY_ELEMENT('a', in, array, 3);
    EXPECT_CHAR_ARRAY_ELEMENT('b', in, array, 3);

    EXPECT_CHAR_ARRAY_ELEMENT('w', not in, array, 3);
    EXPECT_CHAR_ARRAY_ELEMENT('c', not in, array, 2);

    char *cstr = "axy"; // This works because cstrs are '\0' terminated

    EXPECT_CHAR_ARRAY_ELEMENT('a', in, cstr, -1);
    EXPECT_CHAR_ARRAY_ELEMENT('z', not in, cstr, -1);
}
