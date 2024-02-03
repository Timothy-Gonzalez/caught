#include <stdbool.h>

#include "state.h"
#include "config.h"
#ifndef CAUGHT_ASSERTIONS
#define CAUGHT_ASSERTIONS

typedef struct caught_internal_assertion
{
    const char *file;
    const int line;
    const char *call;
    char *expected;
    char *got;
    int pass;
} caught_internal_assertion;

void caught_internal_fancy_str(const char *str);
void caught_internal_handle_assertion(caught_internal_assertion *assertion);

int caught_internal_expect_equal_ptr(caught_internal_assertion *assertion, void *expected, void *got);
int caught_internal_expect_equal_bool(caught_internal_assertion *assertion, bool expected, bool got);
int caught_internal_expect_equal_int(caught_internal_assertion *assertion, int expected, int got);
int caught_internal_expect_equal_char(caught_internal_assertion *assertion, char expected, char got);
int caught_internal_expect_equal_str(caught_internal_assertion *assertion, char *expected, char *got);

#define CAUGHT_INTERNAL_MAKE_ASSERTION(func_name, expected, got) \
    caught_internal_assertion caught_assertion = {               \
        .file = __FILE__,                                        \
        .line = __LINE__,                                        \
        .call = func_name "( " expected ", " got " )",           \
    };

#define CAUGHT_INTERNAL_READ_EXPECTED_AND_GET(type, format, expected_exp, got_exp) \
    type caught_internal_expected = (expected_exp);                                \
    type caught_internal_got = (got_exp);                                          \
    asprintf(&assertion.expected, format, caught_internal_expected);               \
    asprintf(&assertion.got, format, caught_internal_got);

#define CAUGHT_INTERNAL_PASS_ASSERTION(pass_exp)             \
    caught_assertion.pass = (pass_exp);                      \
    if (!caught_assertion.pass)                              \
    {                                                        \
        caught_internal_handle_assertion(&caught_assertion); \
        return;                                              \
    }                                                        \
    caught_internal_handle_assertion(&caught_assertion);

#define CAUGHT_INTERNAL_EXPECT_HANDLE(func, handler, expected_exp, got_exp)               \
    do                                                                                    \
    {                                                                                     \
        CAUGHT_INTERNAL_MAKE_ASSERTION(func, #expected_exp, #got_exp)                     \
        CAUGHT_INTERNAL_PASS_ASSERTION(handler(&caught_assertion, expected_exp, got_exp)) \
    } while (0)

#define EXPECT_EQUAL_PTR(expected_exp, got_exp) \
    CAUGHT_INTERNAL_EXPECT_HANDLE("EXPECT_EQUAL_PTR", caught_internal_expect_equal_ptr, expected_exp, got_exp)
#define EXPECT_EQUAL_BOOL(expected_exp, got_exp) \
    CAUGHT_INTERNAL_EXPECT_HANDLE("EXPECT_EQUAL_BOOL", caught_internal_expect_equal_bool, expected_exp, got_exp)
#define EXPECT_EQUAL_INT(expected_exp, got_exp) \
    CAUGHT_INTERNAL_EXPECT_HANDLE("EXPECT_EQUAL_INT", caught_internal_expect_equal_int, expected_exp, got_exp)
#define EXPECT_EQUAL_CHAR(expected_exp, got_exp) \
    CAUGHT_INTERNAL_EXPECT_HANDLE("EXPECT_EQUAL_CHAR", caught_internal_expect_equal_char, expected_exp, got_exp)
#define EXPECT_EQUAL_STR(expected_exp, got_exp) \
    CAUGHT_INTERNAL_EXPECT_HANDLE("EXPECT_EQUAL_STR", caught_internal_expect_equal_str, expected_exp, got_exp)

#define CAUGHT_INTERNAL_READ_EXPECTED_AND_GET(type, format, expected_exp, got_exp) \
    type caught_internal_expected = (expected_exp);                                \
    type caught_internal_got = (got_exp);                                          \
    asprintf(&assertion.expected, format, caught_internal_expected);               \
    asprintf(&assertion.got, format, caught_internal_got);

#endif
