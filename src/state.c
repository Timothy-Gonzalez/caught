#include <stdlib.h>

#include "state.h"
#include "mocks.h"

struct caught_internal_t caught_internal_state = {
    .assertions = 0,
    .passed_assertions = 0,
    .tests = NULL,
    .tests_num = 0,
    .tests_capacity = 0,
    .mocked_stdout_pipe = {-1, -1},
    .original_stdout = -1,
};

void caught_internal_cleanup_state()
{
    if (caught_internal_state.tests)
    {
        free(caught_internal_state.tests);
        caught_internal_state.tests = NULL;
        caught_internal_state.tests_num = 0;
        caught_internal_state.tests_capacity = 0;
    }

    if (caught_internal_state.original_stdout != -1)
    {
        RESTORE_STDOUT();
    }
}
