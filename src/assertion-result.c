#include <stddef.h>
#include <stdio.h>

#include "output.h"
#include "state.h"

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

// Processes the assertion result by updating the internal state & outputting result of assertion if needed.
// Finally, returns true if assertion failed and test should exit
bool caught_internal_handle_assertion_result(caught_internal_assertion_result assertion_result)
{
    caught_internal_state.assertions += 1;
    if (assertion_result.pass)
    {
        caught_internal_state.passed_assertions += 1;
    }

    int show_regardless_of_pass = 0;

#ifdef EXPLICIT_ASSERTION_PASS
    show_regardless_of_pass = 1;
#endif

    if (!assertion_result.pass || show_regardless_of_pass)
    {
        caught_output_assertion_result(assertion_result);
    }

    free(assertion_result.lhs);
    free(assertion_result.rhs);

    return !assertion_result.pass;
}
