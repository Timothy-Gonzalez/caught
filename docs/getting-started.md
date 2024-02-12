# Other Resources

- [Getting Started](#getting-started)
- [API Reference](./api-reference.md)
- [Custom Assertions](./custom-assertions.md)

# Getting Started

Once you've got [caught installed](../README.md#installation), writing tests is easy!

Tests can be defined using a `TEST` block, and specifiying the name:

```c
#include "caught.h"

TEST("a very cool test") {
    // your test content here
}
```

Of course, an empty test isn't very useful, so you'll want to add assertions.

## Basic Assertions

Basic assertions follow the format `EXPECT_[TYPE](actual, operator, expected)`.

If I wanted to test my factorial function:
```c
int factorial(n)
{
    return n <= 1 ? 1 : factorial(n - 1) * n;
}
```

I could write a test like this:

```c
#include "caught.h"

TEST("factorial")
{
    EXPECT_INT(factorial(1), ==, 1);
    EXPECT_INT(factorial(2), ==, 2);
    EXPECT_INT(factorial(3), ==, 6);
    EXPECT_INT(factorial(4), ==, 24);
    EXPECT_INT(factorial(5), ==, 120);
}
```

Other operators are also supported:
```c
#include "caught.h"

TEST("other operators")
{
    EXPECT_INT(6 * 9, !=, 42);

    EXPECT_INT(734, >, 5);
    EXPECT_INT(-411, <, -1);

    EXPECT_INT(6, >=, 5);
    EXPECT_INT(4, <=, 7);
}
```

## Truth Assertions

Basic truth assertions can also be written, using `EXPECT`. Simply, it expects the value within to be `true`.
There's also the inverse variant, `EXPECT_NOT` which expects false.

```c
#include "caught.h"

TEST("a passing test") {
    EXPECT(true);
    EXPECT(1 + 1 == 2);

    EXPECT_NOT(false);
    EXPECT_NOT(6 * 9 == 42);
}
```

These assertions are useful when Caught doesn't have a typed assertion for what you want.

## Pointer Assertions

When working with pointers, derefrencing them all the time can be annoying. Caught provides an alternative:

```c
TEST("pointers")
{
    int array[] = {1, 5};
    int array2[] = {1, 5};

    // Instead of writing:
    EXPECT_PTR(array, != NULL);
    EXPECT_PTR(array2, != NULL);
    EXPECT_INT(*array, == *array2);

    // You can write:
    EXPECT_INT_PTR(array, == array2);
}
```

Pointers will be checked against null, and then deref'd to compare values.

## Array Assertions

The same standard operations can also be run on arrays. Note that a fourth length argument is required.

The inequality operators (`>`, `<` `>=`, `<=`) test each pair of elements,
so `{1,2,3} > {0, 1, 2}` but `{2,2,2} > {0, 1, 5}` is not.

```c
TEST("arrays")
{
    int array[] = {1, 2, 3, 4};
    int array2[] = {1, 2, 3, 4};
    int array3[] = {2, 3, 4, 5};

    EXPECT_INT_ARRAY(array, ==, array2, 4);
    EXPECT_INT_ARRAY(array, >=, array2, 4);
    EXPECT_INT_ARRAY(array, <=, array2, 4);
    array[3] = -1;
    EXPECT_INT_ARRAY(array, !=, array2, 4);
    EXPECT_INT_ARRAY(array, ==, array2, 3);

    EXPECT_INT_ARRAY(array2, <, array3, 4);
    EXPECT_INT_ARRAY(array3, >, array2, 4);
}
```

## In Operator

Caught also provides a convenience `in` and `not in` operator you can use with strings and arrays:

```c
TEST("str - in")
{
    EXPECT_STR("abc", in, "abcdefghijk");
    EXPECT_STR("123", not in, "45612");

    EXPECT_INT_ARRAY_ELEMENT(1, in, {1, 2, 3});
    EXPECT_INT_ARRAY_ELEMENT(4, not in, {1, 2, 3});
}
```

## Match Operator

Caught also provides a convenience `match` and `not match` operator you can use with strings, where `$specifier` is used to match things.

See the [match operator](./api-reference.md#match-and-not-match) in the API reference for more details.

```c
TEST("str - match")
{
    EXPECT_STR("abc", match, "abc");
    EXPECT_STR("a a char", match, "a $c char");
    EXPECT_STR("a 123 number", match, "a $i number");
    EXPECT_STR("a 123.456 float", match, "a $f float");
    EXPECT_STR("the alphabet match", match, "the $a match");
    EXPECT_STR("the word123 match", match, "the $w match");
    EXPECT_STR("a very long string", match, "a $s string");
    EXPECT_STR("a literal $ can work", match, "a literal $$ can work");

    EXPECT_STR("a 123c number char", match, "a $i$c number char");
    EXPECT_STR("two nice looking - awesome beings strings", match, "two $s - $s strings");

    EXPECT_STR("abc", not match, "def");
    EXPECT_STR("the char is missing: ", not match, "the char is missing: $c");
    EXPECT_STR("the no alpha 123 match", not match, "the no alpha $a match");
    EXPECT_STR("the no whitespace \t match", not match, "the no whitespace $w match");
    EXPECT_STR("the string is missing: ", not match, "the string is missing: $s");
    EXPECT_STR("not a match", not match, "not a $s match");
    EXPECT_STR("too greedy", not match, "$s$s");
}
```

## Exit & Signal Assertions

If your code exits with a exit or signal, you can test it like this:

```c
TEST("exit and signal")
{
    EXPECT_EXIT(EXIT_SUCCESS, {
        exit(EXIT_SUCCESS);
    });

    EXPECT_INT(1 + 1, ==, 2); // This still runs

        EXPECT_SIGNAL(SIGSEGV, {
        int *ptr = NULL;
        ptr[1] = 123; // BAD!
    });

    EXPECT_INT(2 + 2, ==, 4); // This still runs
}
```

## Mocking

Caught provides the ability to mock `stdin` and `stdout`.
This can be useful when you can't or don't want to change your logging for testing.

```c
EST("stdin - hello world")
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
```

```c
TEST("stdout - hello world")
{
    MOCK_STDOUT();
    puts("Hello, world!");
    char *out = RESTORE_STDOUT();
    EXPECT_STR(out, ==, "Hello, world!\n");
    free(out);
}
```

## Custom Assertions

If Caught doesn't provide an assertion you need, you can always implement one yourself.

See [how to make custom assertions here](./custom-assertions.md).

## Final Notes

That's the basics of Caught.
It's also recommended to take a look at the [API Reference](./api-reference.md) for a full list of what Caught has to offer.

Have fun writing tests!
