#include <stdbool.h>

#ifndef CAUGHT_ASSERTION_RESULT
#define CAUGHT_ASSERTION_RESULT

enum caught_operator
{
    CAUGHT_OP_EQUAL,
    CAUGHT_OP_NOT_EQUAL,
    CAUGHT_OP_LESS_THAN,
    CAUGHT_OP_GREATER_THAN,
    CAUGHT_OP_LESS_THAN_EQ,
    CAUGHT_OP_GREATER_THAN_EQ,
    CAUGHT_OP_IN,
    CAUGHT_OP_NOT_IN,
    CAUGHT_OP_MATCH,
    CAUGHT_OP_NOT_MATCH,
};

static char *CAUGHT_OPERATOR_STRS[] = {
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
    "in",
    "not in",
    "match",
    "not match",
};

static char *CAUGHT_OPERATOR_TO_BES[] = {
    "to be",
    "to not be",
    "to be less than",
    "to be greater than",
    "to be <= to",
    "to be >= to",
    "to be in",
    "to not be in",
    "to match",
    "to not match",
};

enum caught_operator
caught_str_to_operator(char *str);
const char *caught_operator_to_str(enum caught_operator operator);
const char *caught_operator_to_to_be_statement(enum caught_operator operator);

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

#endif
