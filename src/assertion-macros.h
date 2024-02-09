#ifndef CAUGHT_ASSERTIONS
#define CAUGHT_ASSERTIONS

#include "assertion-result.h"
#include "formatters.h"
#include "evaluators.h"
#include "state.h"

// This is used by every expect define to handle taking lhs, op, rhs, & send them into their handlers.
// These handlers then determine how to display (format) the passed data, and whether the assertion passed (comparators).
// Finally, these results are combined into a assertion result and sent to the result handler, which outputs and keeps track
// of assertions accordingly.
//
// Var args (...) are passed directly to formatter & evaluator, allowing for custom options for assertions.
// See array assertions for an example.
//
// Note: do while is required to have non-conflicting scope if multiple assertions are used
#define CAUGHT_INTERNAL_EXPECT_HANDLE(func_postfix, type_exp, lhs_exp, operator_exp, rhs_exp, evaluator, formatter, ...)   \
    do                                                                                                                     \
    {                                                                                                                      \
        type_exp caught_internal_lhs = (lhs_exp);                                                                          \
        type_exp caught_internal_rhs = (rhs_exp);                                                                          \
        caught_internal_assertion_result caught_internal_assertion_result = {                                              \
            .file = __FILE__,                                                                                              \
            .line = __LINE__,                                                                                              \
            .expression = "EXPECT_" #func_postfix "( " #lhs_exp " " #operator_exp " " #rhs_exp " )",                       \
            .lhs = formatter(caught_internal_lhs, ##__VA_ARGS__),                                                          \
            .rhs = formatter(caught_internal_rhs, ##__VA_ARGS__),                                                          \
            .operator= caught_str_to_operator(#operator_exp),                                                              \
            .pass =                                                                                                        \
                evaluator(caught_internal_lhs, caught_str_to_operator(#operator_exp), caught_internal_rhs, ##__VA_ARGS__), \
        };                                                                                                                 \
        if (caught_internal_handle_assertion_result(caught_internal_assertion_result))                                     \
        {                                                                                                                  \
            return;                                                                                                        \
        }                                                                                                                  \
    } while (0);

// Convience mapping to expect handle that takes a length and passed it as a option
#define CAUGHT_INTERNAL_EXPECT_ARRAY_HANDLE(func_postfix, type, lhs, op, rhs, length_exp, evaluator, formatter)             \
    do                                                                                                                      \
    {                                                                                                                       \
        size_t caught_internal_array_length = length_exp;                                                                   \
        CAUGHT_INTERNAL_EXPECT_HANDLE(func_postfix, type, lhs, op, rhs, evaluator, formatter, caught_internal_array_length) \
    } while (0);

#define EXPECT_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(PTR, void *, lhs, op, rhs, caught_internal_evaluator_ptr, caught_internal_formatter_ptr)
#define EXPECT_PTR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(PTR_PTR, void **, lhs, op, rhs, caught_internal_evaluator_ptr_ptr, caught_internal_formatter_ptr_ptr)
#define EXPECT_PTR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_ARRAY_HANDLE(PTR_ARRAY, void **, lhs, op, rhs, length, caught_internal_evaluator_ptr_array, caught_internal_formatter_ptr_array)

#define EXPECT_BOOL(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(BOOL, bool, lhs, op, rhs, caught_internal_evaluator_bool, caught_internal_formatter_bool)
#define EXPECT_BOOL_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(BOOL_PTR, bool *, lhs, op, rhs, caught_internal_evaluator_bool_ptr, caught_internal_formatter_bool_ptr)
#define EXPECT_BOOL_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_ARRAY_HANDLE(BOOL_ARRAY, bool *, lhs, op, rhs, length, caught_internal_evaluator_bool_array, caught_internal_formatter_bool_array)

#define EXPECT_INT(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(INT, int, lhs, op, rhs, caught_internal_evaluator_int, caught_internal_formatter_int)
#define EXPECT_INT_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(INT_PTR, int *, lhs, op, rhs, caught_internal_evaluator_int_ptr, caught_internal_formatter_int_ptr)
#define EXPECT_INT_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_ARRAY_HANDLE(INT_ARRAY, int *, lhs, op, rhs, length, caught_internal_evaluator_int_array, caught_internal_formatter_int_array)

#define EXPECT_CHAR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(CHAR, char, lhs, op, rhs, caught_internal_evaluator_char, caught_internal_formatter_char)
#define EXPECT_CHAR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(CHAR_PTR, char *, lhs, op, rhs, caught_internal_evaluator_char_ptr, caught_internal_formatter_char_ptr)
#define EXPECT_CHAR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_ARRAY_HANDLE(CHAR_ARRAY, char *, lhs, op, rhs, length, caught_internal_evaluator_char_array, caught_internal_formatter_char_array)

#define EXPECT_STR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(STR, char *, lhs, op, rhs, caught_internal_evaluator_str, caught_internal_formatter_str)
#define EXPECT_STR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(STR_PTR, char **, lhs, op, rhs, caught_internal_evaluator_str_ptr, caught_internal_formatter_str_ptr)
#define EXPECT_STR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_ARRAY_HANDLE(STR_ARRAY, char **, lhs, op, rhs, length, caught_internal_evaluator_str_array, caught_internal_formatter_str_array)

#define CAUGHT_INTERNAL_EXPECT_TERMINATE_HANDLE(func_name, expected_status, execute_block)                                               \
    do                                                                                                                                   \
    {                                                                                                                                    \
        CAUGHT_INTERNAL_FORK(execute_block)                                                                                              \
        CAUGHT_INTERNAL_EXPECT_HANDLE(func_name, caught_internal_process_status, caught_internal_fork_child_status, ==, expected_status, \
                                      caught_internal_evaluator_exit_status, caught_internal_formatter_exit_status);                     \
    } while (0);

#define EXPECT_EXIT(expected_status, execute_block) \
    CAUGHT_INTERNAL_EXPECT_TERMINATE_HANDLE(EXIT, create_caught_internal_process_status(0, expected_status), execute_block)

#define EXPECT_SIGNAL(expected_status, execute_block) \
    CAUGHT_INTERNAL_EXPECT_TERMINATE_HANDLE(EXIT, create_caught_internal_process_status(1, expected_status), execute_block)

#endif
