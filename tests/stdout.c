// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("stdout - hello world")
{
    MOCK_STDOUT();
    puts("Hello, world!");
    char *out = RESTORE_STDOUT();
    EXPECT_STR(out, ==, "Hello, world!\n");
    free(out);
}

TEST("stdout - a lot of text")
{
    MOCK_STDOUT();
    puts("The answer to life,");
    puts("the universe,");
    puts("and everything,");
    puts("is 42.");
    char *out = RESTORE_STDOUT();
    EXPECT_STR(out, ==, "The answer to life,\nthe universe,\nand everything,\nis 42.\n");
    free(out);
}

TEST("stdout - with expect")
{
    MOCK_STDOUT();
    puts("This is fun!");
    EXPECT_INT(1 + 1, ==, 2);
    char *out = RESTORE_STDOUT();
    EXPECT_STR(out, ==, "This is fun!\n");
    free(out);
}
