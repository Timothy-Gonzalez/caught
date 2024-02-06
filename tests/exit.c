// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("exit - success")
{
    EXPECT_EXIT(EXIT_SUCCESS, {
        exit(EXIT_SUCCESS);
    });

    EXPECT_INT(1 + 1, ==, 2); // This still runs
}

TEST("exit - failure")
{
    EXPECT_EXIT(EXIT_FAILURE, {
        exit(EXIT_FAILURE);
    });

    EXPECT_INT(1 + 1, ==, 2); // This still runs
}
