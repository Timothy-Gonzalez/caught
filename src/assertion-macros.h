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
// Note: do while is required to have non-conflicting scope if multiple assertions are used
#define CAUGHT_INTERNAL_EXPECT_HANDLE(func_postfix, type_lhs, type_rhs, lhs_exp, operator_exp, rhs_exp, evaluated, formated_lhs, formated_rhs, ...) \
    do                                                                                                                                              \
    {                                                                                                                                               \
        type_lhs caught_internal_lhs = (lhs_exp);                                                                                                   \
        type_rhs caught_internal_rhs = (rhs_exp);                                                                                                   \
        enum caught_operator caught_internal_op = caught_str_to_operator(#operator_exp);                                                            \
        caught_internal_assertion_result caught_internal_assertion_result = {                                                                       \
            .file = __FILE__,                                                                                                                       \
            .line = __LINE__,                                                                                                                       \
            .expression = "EXPECT_" #func_postfix "( " #lhs_exp " " #operator_exp " " #rhs_exp " )",                                                \
            .lhs = formated_lhs,                                                                                                                    \
            .rhs = formated_rhs,                                                                                                                    \
            .operator= caught_internal_op,                                                                                                          \
            .pass = evaluated,                                                                                                                      \
        };                                                                                                                                          \
        if (caught_internal_handle_assertion_result(caught_internal_assertion_result))                                                              \
        {                                                                                                                                           \
            return;                                                                                                                                 \
        }                                                                                                                                           \
    } while (0);

// Convience mapping for when type & formatter are both the same
#define CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(func_postfix, type, lhs, op, rhs, evaluator, formatter)                                              \
    CAUGHT_INTERNAL_EXPECT_HANDLE(func_postfix, type, type, lhs, op, rhs, evaluator(caught_internal_lhs, caught_internal_op, caught_internal_rhs), \
                                  formatter(caught_internal_lhs), formatter(caught_internal_rhs))

// Convience mapping for passing length as an argument, and when type & formatter are both the same
#define CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(func_postfix, type, lhs, op, rhs, length_exp, evaluator, formatter)                              \
    do                                                                                                                                       \
    {                                                                                                                                        \
        size_t caught_internal_array_length = length_exp;                                                                                    \
        CAUGHT_INTERNAL_EXPECT_HANDLE(func_postfix, type, type, lhs, op, rhs,                                                                \
                                      evaluator(caught_internal_lhs, caught_internal_op, caught_internal_rhs, caught_internal_array_length), \
                                      formatter(caught_internal_lhs, caught_internal_array_length),                                          \
                                      formatter(caught_internal_rhs, caught_internal_array_length))                                          \
    } while (0);

#define EXPECT_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(PTR, void *, lhs, op, rhs, caught_internal_evaluator_ptr, caught_internal_formatter_ptr)
#define EXPECT_PTR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(PTR_PTR, void **, lhs, op, rhs, caught_internal_evaluator_ptr_ptr, caught_internal_formatter_ptr_ptr)
#define EXPECT_PTR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(PTR_ARRAY, void **, lhs, op, rhs, length, caught_internal_evaluator_ptr_array, caught_internal_formatter_ptr_array)

#define EXPECT_BOOL(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(BOOL, bool, lhs, op, rhs, caught_internal_evaluator_bool, caught_internal_formatter_bool)
#define EXPECT_BOOL_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(BOOL_PTR, bool *, lhs, op, rhs, caught_internal_evaluator_bool_ptr, caught_internal_formatter_bool_ptr)
#define EXPECT_BOOL_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(BOOL_ARRAY, bool *, lhs, op, rhs, length, caught_internal_evaluator_bool_array, caught_internal_formatter_bool_array)

#define EXPECT_INT(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(INT, int, lhs, op, rhs, caught_internal_evaluator_int, caught_internal_formatter_int)
#define EXPECT_INT_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(INT_PTR, int *, lhs, op, rhs, caught_internal_evaluator_int_ptr, caught_internal_formatter_int_ptr)
#define EXPECT_INT_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(INT_ARRAY, int *, lhs, op, rhs, length, caught_internal_evaluator_int_array, caught_internal_formatter_int_array)

#define EXPECT_CHAR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(CHAR, char, lhs, op, rhs, caught_internal_evaluator_char, caught_internal_formatter_char)
#define EXPECT_CHAR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(CHAR_PTR, char *, lhs, op, rhs, caught_internal_evaluator_char_ptr, caught_internal_formatter_char_ptr)
#define EXPECT_CHAR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(CHAR_ARRAY, char *, lhs, op, rhs, length, caught_internal_evaluator_char_array, caught_internal_formatter_char_array)

#define EXPECT_STR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(STR, char *, lhs, op, rhs, caught_internal_evaluator_str, caught_internal_formatter_str)
#define EXPECT_STR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(STR_PTR, char **, lhs, op, rhs, caught_internal_evaluator_str_ptr, caught_internal_formatter_str_ptr)
#define EXPECT_STR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(STR_ARRAY, char **, lhs, op, rhs, length, caught_internal_evaluator_str_array, caught_internal_formatter_str_array)

#define CAUGHT_INTERNAL_EXPECT_TERMINATE_HANDLE(func_name, expected_status, execute_block)                                                       \
    do                                                                                                                                           \
    {                                                                                                                                            \
        CAUGHT_INTERNAL_FORK(execute_block)                                                                                                      \
        CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(func_name, caught_internal_process_status, caught_internal_fork_child_status, ==, expected_status, \
                                              caught_internal_evaluator_exit_status, caught_internal_formatter_exit_status);                     \
    } while (0);

#define EXPECT_EXIT(expected_status, execute_block) \
    CAUGHT_INTERNAL_EXPECT_TERMINATE_HANDLE(EXIT, create_caught_internal_process_status(0, expected_status), execute_block)

#define EXPECT_SIGNAL(expected_status, execute_block) \
    CAUGHT_INTERNAL_EXPECT_TERMINATE_HANDLE(EXIT, create_caught_internal_process_status(1, expected_status), execute_block)

#endif
