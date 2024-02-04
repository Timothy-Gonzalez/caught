#include <stdbool.h>

#include "state.h"
#include "config.h"
#include "evaluators.h"
#include "formatters.h"

#ifndef CAUGHT_ASSERTIONS
#define CAUGHT_ASSERTIONS

typedef struct caught_internal_assertion_result
{
    const char *file;
    const int line;
    const char *expression;
    char *lhs;
    char *rhs;
    enum caught_operator operator;
    bool pass;
} caught_internal_assertion_result;

bool caught_internal_handle_assertion_result(caught_internal_assertion_result assertion_result);

// This is used by every expect define to handle taking lhs, op, rhs, & send them into their handlers.
// These handlers then determine how to display (format) the passed data, and whether the assertion passed (comparators).
// Finally, these results are combined into a assertion result and sent to the result handler, which outputs and keeps track
// of assertions accordingly.
//
// Note: do while is required to have non-conflicting scope if multiple assertions are used
#define CAUGHT_INTERNAL_EXPECT_HANDLE(func_postfix, type_exp, lhs_exp, operator_exp, rhs_exp, assertion_handler, formatter) \
    do                                                                                                                      \
    {                                                                                                                       \
        type_exp caught_internal_lhs = (lhs_exp);                                                                           \
        type_exp caught_internal_rhs = (rhs_exp);                                                                           \
        caught_internal_assertion_result caught_internal_assertion_result = {                                               \
            .file = __FILE__,                                                                                               \
            .line = __LINE__,                                                                                               \
            .expression = "EXPECT_" #func_postfix "( " #lhs_exp " " #operator_exp " " #rhs_exp " )",                        \
            .lhs = formatter(caught_internal_lhs),                                                                          \
            .rhs = formatter(caught_internal_rhs),                                                                          \
            .operator= caught_str_to_operator(#operator_exp),                                                               \
            .pass = assertion_handler(caught_internal_lhs, caught_str_to_operator(#operator_exp), caught_internal_rhs),     \
        };                                                                                                                  \
        if (caught_internal_handle_assertion_result(caught_internal_assertion_result))                                      \
        {                                                                                                                   \
            return;                                                                                                         \
        }                                                                                                                   \
    } while (0)

#define EXPECT_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(PTR, void *, lhs, op, rhs, caught_internal_evaluator_ptr, caught_internal_formatter_ptr)
#define EXPECT_BOOL(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(BOOL, bool, lhs, op, rhs, caught_internal_evaluator_int, caught_internal_formatter_bool)
#define EXPECT_INT(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(INT, int, lhs, op, rhs, caught_internal_evaluator_int, caught_internal_formatter_int)
#define EXPECT_CHAR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(CHAR, char, lhs, op, rhs, caught_internal_evaluator_char, caught_internal_formatter_char)
#define EXPECT_STR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(STR, char *, lhs, op, rhs, caught_internal_evaluator_str, caught_internal_formatter_str)

#endif
