#include <stddef.h>
#include <stdio.h>

#include "assertions.h"
#include "output.h"

// Processes the assertion result by updating the internal state & outputting result of assertion if needed.
// Finally, returns true if assertion failed and test should exit
bool caught_internal_handle_assertion_result(caught_internal_assertion_result assertion_result)
{
    caught_internal.assertions += 1;
    if (assertion_result.pass)
    {
        caught_internal.passed_assertions += 1;
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
