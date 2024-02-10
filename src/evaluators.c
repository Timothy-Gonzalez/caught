#include "evaluators.h"
#include "output.h"
#include <stdlib.h>
#include <stdio.h>

// Evaluators take in a left hand size, operator, and right hand side
// they then evaluate the result of that expression
// CAUGHT_GENERATE_GENERIC_EVALUATOR just uses the default operators (==, <=, >=, ...)
// but more advanced definitions are needed for things like strings

bool caught_internal_evaluator_ptr(void *lhs, enum caught_operator operator, void * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_ptr_ptr(void **lhs, enum caught_operator operator, void ** rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_ptr(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_ptr_array(void **lhs, enum caught_operator operator, void ** rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, operator, rhs, length, caught_internal_evaluator_ptr);
}

bool caught_internal_evaluator_bool(bool lhs, enum caught_operator operator, bool rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_bool_ptr(bool *lhs, enum caught_operator operator, bool * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_bool(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_bool_array(bool *lhs, enum caught_operator operator, bool * rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, operator, rhs, length, caught_internal_evaluator_bool);
}

bool caught_internal_evaluator_int(int lhs, enum caught_operator operator, int rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_int_ptr(int *lhs, enum caught_operator operator, int * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_int(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_int_array(int *lhs, enum caught_operator operator, int * rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, operator, rhs, length, caught_internal_evaluator_int);
}

bool caught_internal_evaluator_char(char lhs, enum caught_operator operator, char rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_char_ptr(char *lhs, enum caught_operator operator, char * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_int(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_char_array(char *lhs, enum caught_operator operator, char * rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY_ALLOW_NULL_TERMINATOR(lhs, operator, rhs, length, caught_internal_evaluator_char, '\0');
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
    case CAUGHT_OP_IN:
        return !null_exists && (strstr(rhs, lhs) != NULL);
    case CAUGHT_OP_NOT_IN:
        return null_exists || (strstr(rhs, lhs) == NULL);
    default:
        caught_output_errorf("Cannot compare strings with %s, only == and != are supported!", caught_operator_to_str(operator));
        return false;
    }
}
bool caught_internal_evaluator_str_ptr(char **lhs, enum caught_operator operator, char ** rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_str(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_str_array(char **lhs, enum caught_operator operator, char ** rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY_ALLOW_NULL_TERMINATOR(lhs, operator, rhs, length, caught_internal_evaluator_str, NULL);
}

bool caught_internal_evaluator_exit_status(caught_internal_process_status lhs, enum caught_operator operator, caught_internal_process_status rhs)
{
    if (operator!= CAUGHT_OP_EQUAL)
    {
        caught_output_errorf("Cannot compare exit statuses with %s, only == is supported!", caught_operator_to_str(operator));
    }

    return lhs.type == rhs.type && lhs.status == rhs.status;
}
