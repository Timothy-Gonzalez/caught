// NOTE: The location of this include might differ in your code depending on location
// For example, it could be: #include "caught.h"
#include "../src/caught.h"

TEST("stdin - hello world")
{
    MOCK_STDIN("Hello, world!\n");
    char *line = NULL;
    size_t line_cap = 0;

    ssize_t len = getline(&line, &line_cap, stdin);

    RESTORE_STDIN();

    EXPECT_INT(len, !=, -1);
    EXPECT_STR(line, ==, "Hello, world!\n");

    free(line);
}

TEST("stdin - with expect exit & MOCK_STDOUT")
{
    MOCK_STDIN("It's complicated\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, {
        char *line = NULL;
        size_t line_cap = 0;
        ssize_t len = getline(&line, &line_cap, stdin);

        if (len == -1)
        {
            exit(1);
        }

        printf("%s", line);
        free(line);

        exit(0);
    });

    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    EXPECT_STR(out, ==, "It's complicated\n");

    free(out);
}
