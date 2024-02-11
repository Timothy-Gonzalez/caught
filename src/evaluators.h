#include <stdbool.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>
#include "fork.h"
#include "assertion-result.h"

#ifndef CAUGHT_EVALUATORS
#define CAUGHT_EVALUATORS

bool caught_internal_evaluator_ptr(void *lhs, enum caught_operator operator, void * rhs);
bool caught_internal_evaluator_ptr_ptr(void **lhs, enum caught_operator operator, void ** rhs);
bool caught_internal_evaluator_ptr_array(void **lhs, enum caught_operator operator, void ** rhs, ssize_t length);

bool caught_internal_evaluator_bool(bool lhs, enum caught_operator operator, bool rhs);
bool caught_internal_evaluator_bool_ptr(bool *lhs, enum caught_operator operator, bool * rhs);
bool caught_internal_evaluator_bool_array(bool *lhs, enum caught_operator operator, bool * rhs, ssize_t length);

bool caught_internal_evaluator_int(int lhs, enum caught_operator operator, int rhs);
bool caught_internal_evaluator_int_ptr(int *lhs, enum caught_operator operator, int * rhs);
bool caught_internal_evaluator_int_array(int *lhs, enum caught_operator operator, int * rhs, ssize_t length);

bool caught_internal_evaluator_char(char lhs, enum caught_operator operator, char rhs);
bool caught_internal_evaluator_char_ptr(char *lhs, enum caught_operator operator, char * rhs);
bool caught_internal_evaluator_char_array(char *lhs, enum caught_operator operator, char * rhs, ssize_t length);

bool caught_internal_evaluator_str_match(char *lhs, char *fstr);

bool caught_internal_evaluator_str(char *lhs, enum caught_operator operator, char * rhs);
bool caught_internal_evaluator_str_ptr(char **lhs, enum caught_operator operator, char ** rhs);
bool caught_internal_evaluator_str_array(char **lhs, enum caught_operator operator, char ** rhs, ssize_t length);

bool caught_internal_evaluator_exit_status(caught_internal_process_status lhs, enum caught_operator operator, caught_internal_process_status rhs);

// Uses default operators (==, <=, >=, ...) to compare lhs to rhs
#define CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)                          \
    switch (operator)                                                                  \
    {                                                                                  \
    case CAUGHT_OP_EQUAL:                                                              \
        return lhs == rhs;                                                             \
    case CAUGHT_OP_NOT_EQUAL:                                                          \
        return lhs != rhs;                                                             \
    case CAUGHT_OP_LESS_THAN:                                                          \
        return lhs < rhs;                                                              \
    case CAUGHT_OP_GREATER_THAN:                                                       \
        return lhs > rhs;                                                              \
    case CAUGHT_OP_LESS_THAN_EQ:                                                       \
        return lhs <= rhs;                                                             \
    case CAUGHT_OP_GREATER_THAN_EQ:                                                    \
        return lhs >= rhs;                                                             \
    default:                                                                           \
        caught_output_errorf("Invalid operator %s", caught_operator_to_str(operator)); \
        return false;                                                                  \
    }

// If lhs or rhs in null, return the expected result for that operator
#define CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs)                                                                     \
    if (lhs == NULL || rhs == NULL)                                                                                                          \
    {                                                                                                                                        \
        bool equal = lhs == rhs;                                                                                                             \
        bool equal_operator = (operator== CAUGHT_OP_EQUAL) || (operator== CAUGHT_OP_GREATER_THAN_EQ) || (operator== CAUGHT_OP_LESS_THAN_EQ); \
        return (equal && equal_operator) || (!equal && !equal_operator);                                                                     \
    }

// Uses evaluator on each element of array
#define CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs_exp, op_exp, rhs_exp, length, evaluator)                                \
    if (length <= 0)                                                                                                \
    {                                                                                                               \
        caught_output_errorf("Invalid length of array: %lu", length);                                               \
    }                                                                                                               \
    bool pass_all = true;                                                                                           \
    bool pass_any = false;                                                                                          \
    bool in_op = (op_exp == CAUGHT_OP_IN || op_exp == CAUGHT_OP_NOT_IN);                                            \
    bool pass_any_op = (op_exp == CAUGHT_OP_NOT_EQUAL || op_exp == CAUGHT_OP_IN);                                   \
    int i;                                                                                                          \
    for (i = 0; i < length; ++i)                                                                                    \
    {                                                                                                               \
        bool pass;                                                                                                  \
        if (!in_op)                                                                                                 \
        {                                                                                                           \
            pass = evaluator(lhs_exp[i], op_exp, rhs_exp[i]);                                                       \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            pass = evaluator(*lhs_exp, op_exp == CAUGHT_OP_IN ? CAUGHT_OP_EQUAL : CAUGHT_OP_NOT_EQUAL, rhs_exp[i]); \
        }                                                                                                           \
                                                                                                                    \
        if (pass)                                                                                                   \
        {                                                                                                           \
            pass_any = true;                                                                                        \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            pass_all = false;                                                                                       \
        }                                                                                                           \
    }                                                                                                               \
    return (!pass_any_op && pass_all) || (pass_any_op && pass_any);

#define CAUGHT_GENERATE_EVALUATOR_ARRAY_ALLOW_NULL_TERMINATOR(lhs, op, rhs, length, evaluator, terminator) \
    if (length < 0)                                                                                        \
    {                                                                                                      \
        length = -1;                                                                                       \
        do                                                                                                 \
        {                                                                                                  \
            length += 1;                                                                                   \
        } while (rhs[length] != terminator);                                                               \
        length += 1; /* Include terminator */                                                              \
    }                                                                                                      \
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, op, rhs, length, evaluator)

#endif
