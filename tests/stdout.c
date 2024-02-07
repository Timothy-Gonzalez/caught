// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("stdout - hello world")
{
    MOCK_STDOUT();
    puts("Hello, world!");
    char *stdout = RESTORE_STDOUT();
    EXPECT_STR(stdout, ==, "Hello, world!\n");
}

TEST("stdout - a lot of text")
{
    MOCK_STDOUT();
    puts("The answer to life,");
    puts("the universe,");
    puts("and everything,");
    puts("is 42.");
    char *stdout = RESTORE_STDOUT();
    EXPECT_STR(stdout, ==, "The answer to life,\nthe universe,\nand everything,\nis 42.\n");
}
