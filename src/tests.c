#include "tests.h"
#include "state.h"
#include <stdlib.h>

void caught_add_test(const char *name, void (*handler)())
{
    if (caught_internal_state.tests == NULL)
    {
        caught_internal_state.tests_capacity = CAUGHT_INTERNAL_TESTS_INITIAL_CAPACITY;
        caught_internal_state.tests = malloc(sizeof(caught_internal_test) * caught_internal_state.tests_capacity);
    }
    else if (caught_internal_state.tests_num == caught_internal_state.tests_capacity)
    {
        caught_internal_state.tests_capacity *= CAUGHT_INTERNAL_TESTS_CAPACITY_GROW_RATE;
        caught_internal_state.tests = realloc(caught_internal_state.tests, sizeof(caught_internal_test) * caught_internal_state.tests_capacity);
    }

    const caught_internal_test test = {
        .name = name,
        .handler = handler,
    };

    caught_internal_state.tests[caught_internal_state.tests_num++] = test;
}
