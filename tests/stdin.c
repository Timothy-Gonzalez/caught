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

TEST("stdin - no newline, only EOF")
{
    MOCK_STDIN("Hello, world!"
               " This is a really long string that will keep going on and on"
               "and on for a while but never ever ever have a newline to be breaking up the flow of life.");
    char *line = NULL;
    size_t line_cap = 0;
    ssize_t len = 0;
    char buffer[500];
    size_t buffer_size = 0;

    while ((len = getline(&line, &line_cap, stdin)) != -1)
    {
        memcpy(buffer + buffer_size, line, len);
        buffer_size += len;
    }
    buffer[buffer_size] = '\0';

    RESTORE_STDIN();

    EXPECT_STR(buffer, ==, "Hello, world!"
                           " This is a really long string "
                           "that will keep going on and on"
                           "and on for a while but never ever "
                           "ever have a newline to be breaking "
                           "up the flow of life.");

    free(line);
}
