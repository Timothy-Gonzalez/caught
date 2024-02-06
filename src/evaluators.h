#include <stdbool.h>
#include <string.h>
#include "fork.h"

#ifndef CAUGHT_EVALUATORS
#define CAUGHT_EVALUATORS

enum caught_operator
{
    CAUGHT_OP_EQUAL,
    CAUGHT_OP_NOT_EQUAL,
    CAUGHT_OP_LESS_THAN,
    CAUGHT_OP_GREATER_THAN,
    CAUGHT_OP_LESS_THAN_EQ,
    CAUGHT_OP_GREATER_THAN_EQ,
};

static char *CAUGHT_OPERATOR_STRS[] = {
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
};

static char *CAUGHT_OPERATOR_TO_BES[] = {
    "to be",
    "to not be",
    "to be less than",
    "to be greater than",
    "to be <= to",
    "to be >= to",
};

enum caught_operator
caught_str_to_operator(char *str);
const char *caught_operator_to_str(enum caught_operator operator);
const char *caught_operator_to_to_be_statement(enum caught_operator operator);

bool caught_internal_evaluator_ptr(void *lhs, enum caught_operator operator, void * rhs);
bool caught_internal_evaluator_ptr_ptr(void **lhs, enum caught_operator operator, void ** rhs);

bool caught_internal_evaluator_bool(bool lhs, enum caught_operator operator, bool rhs);
bool caught_internal_evaluator_bool_ptr(bool *lhs, enum caught_operator operator, bool * rhs);

bool caught_internal_evaluator_int(int lhs, enum caught_operator operator, int rhs);
bool caught_internal_evaluator_int_ptr(int *lhs, enum caught_operator operator, int * rhs);

bool caught_internal_evaluator_char(char lhs, enum caught_operator operator, char rhs);
bool caught_internal_evaluator_char_ptr(char *lhs, enum caught_operator operator, char * rhs);

bool caught_internal_evaluator_str(char *lhs, enum caught_operator operator, char * rhs);
bool caught_internal_evaluator_str_ptr(char **lhs, enum caught_operator operator, char ** rhs);

bool caught_internal_evaluator_exit_status(caught_internal_process_status lhs, enum caught_operator operator, caught_internal_process_status rhs);

// Uses default operators (==, <=, >=, ...) to compare lhs to rhs
#define CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs) \
    switch (operator)                                         \
    {                                                         \
    case CAUGHT_OP_EQUAL:                                     \
        return lhs == rhs;                                    \
    case CAUGHT_OP_NOT_EQUAL:                                 \
        return lhs != rhs;                                    \
    case CAUGHT_OP_LESS_THAN:                                 \
        return lhs < rhs;                                     \
    case CAUGHT_OP_GREATER_THAN:                              \
        return lhs > rhs;                                     \
    case CAUGHT_OP_LESS_THAN_EQ:                              \
        return lhs <= rhs;                                    \
    case CAUGHT_OP_GREATER_THAN_EQ:                           \
        return lhs >= rhs;                                    \
    default:                                                  \
        return false;                                         \
    }

// If lhs or rhs in null, return the expected result for that operator
#define CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs)                                                                     \
    if (lhs == NULL || rhs == NULL)                                                                                                          \
    {                                                                                                                                        \
        bool equal = lhs == rhs;                                                                                                             \
        bool equal_operator = (operator== CAUGHT_OP_EQUAL) || (operator== CAUGHT_OP_GREATER_THAN_EQ) || (operator== CAUGHT_OP_LESS_THAN_EQ); \
        return (equal && equal_operator) || (!equal && !equal_operator);                                                                     \
    }

#endif
