// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("signal - SIGABRT")
{
    EXPECT_SIGNAL(SIGABRT, {
        raise(SIGABRT);
    });

    EXPECT_INT(1 + 1, ==, 2); // This still runs
}

TEST("signal - SIGSEGV")
{
    EXPECT_SIGNAL(SIGSEGV, {
        int *ptr = NULL;
        ptr[1] = 123; // BAD!
    });

    EXPECT_INT(1 + 1, ==, 2); // This still runs
}
