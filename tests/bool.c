// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

#include <stdbool.h>

TEST("bool - basic")
{
    EXPECT_BOOL(true, ==, true);
    EXPECT_BOOL(false, ==, false);
    EXPECT_BOOL(true, !=, false);
    EXPECT_BOOL(false, !=, true);
}

TEST("bool - inequalities")
{
    EXPECT_BOOL(true, >, false);
    EXPECT_BOOL(true, >=, false);
    EXPECT_BOOL(true, >=, true);

    EXPECT_BOOL(false, <, true);
    EXPECT_BOOL(false, <=, true);
    EXPECT_BOOL(false, <=, false);
}

TEST("bool - ptrs")
{
    bool the_truth = true;
    bool the_other_truth = true;
    bool the_lie = false;
    bool the_other_lie = false;
    EXPECT_BOOL_PTR(&the_truth, ==, &the_other_truth);
    EXPECT_BOOL_PTR(&the_lie, ==, &the_other_lie);
    EXPECT_BOOL_PTR(&the_truth, !=, &the_lie);

    EXPECT_BOOL_PTR(NULL, !=, &the_lie);
    EXPECT_BOOL_PTR(&the_lie, !=, NULL);

    EXPECT_BOOL_PTR(&the_truth, >, &the_lie);
    EXPECT_BOOL_PTR(&the_truth, >=, &the_lie);
    EXPECT_BOOL_PTR(&the_truth, >=, &the_other_truth);

    EXPECT_BOOL_PTR(&the_lie, <, &the_truth);
    EXPECT_BOOL_PTR(&the_lie, <=, &the_truth);
    EXPECT_BOOL_PTR(&the_lie, <=, &the_other_lie);
}

TEST("bool - arrays")
{
    bool array[] = {false, false, false};
    bool array2[] = {false, false, false};
    bool array3[] = {true, true, true};

    EXPECT_BOOL_ARRAY(array, ==, array2, 3);
    EXPECT_BOOL_ARRAY(array, >=, array2, 3);
    EXPECT_BOOL_ARRAY(array, <=, array2, 3);
    array[2] = true;
    EXPECT_BOOL_ARRAY(array, !=, array2, 3);
    EXPECT_BOOL_ARRAY(array, ==, array2, 2);

    EXPECT_BOOL_ARRAY(array2, <, array3, 3);
    EXPECT_BOOL_ARRAY(array3, >, array2, 3);
}
