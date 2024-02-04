#include "evaluators.h"
#include <stdlib.h>
#include <stdio.h>

// General purpose converter from string (==) to operator enum (CAUGHT_OP_EQUAL)
enum caught_operator
caught_str_to_operator(char *str)
{
    int len = sizeof(CAUGHT_OPERATOR_STRS) / sizeof(CAUGHT_OPERATOR_STRS[0]);
    int i;
    for (i = 0; i < len; ++i)
    {
        if (strcmp(str, CAUGHT_OPERATOR_STRS[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

// General purpose converter from enum (CAUGHT_OP_EQUAL) to operator string (==)
const char *caught_operator_to_str(enum caught_operator operator)
{
    return CAUGHT_OPERATOR_STRS[operator];
}

// General purpose converter from enum (CAUGHT_OP_EQUAL) to a to be statement (to be, to not be, etc.)
const char *caught_operator_to_to_be_statement(enum caught_operator operator)
{
    return CAUGHT_OPERATOR_TO_BES[operator];
}

// Evaluators take in a left hand size, operator, and right hand side
// they then evaluate the result of that expression
// CAUGHT_GENERATE_GENERIC_EVALUATOR just uses the default operators (==, <=, >=, ...)
// but more advanced definitions are needed for things like strings

bool caught_internal_evaluator_ptr(void *lhs, enum caught_operator operator, void * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR
}
bool caught_internal_evaluator_bool(bool lhs, enum caught_operator operator, bool rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR
}

bool caught_internal_evaluator_int(int lhs, enum caught_operator operator, int rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR
}
bool caught_internal_evaluator_char(char lhs, enum caught_operator operator, char rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR
}
bool caught_internal_evaluator_str(char *lhs, enum caught_operator operator, char * rhs)
{
    bool null_exists = (lhs == NULL) || (rhs == NULL);
    switch (operator)
    {
    case CAUGHT_OP_EQUAL:
        return (null_exists && lhs == rhs) || (!null_exists && strcmp(lhs, rhs) == 0);
    case CAUGHT_OP_NOT_EQUAL:
        return (null_exists && lhs != rhs) || (!null_exists && strcmp(lhs, rhs) != 0);
    default:
        fprintf(stderr, "Cannot compare strings with %s, only == and != are supported!", caught_operator_to_str(operator));
        exit(1);
    }
}
