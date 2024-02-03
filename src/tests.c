#include "tests.h"
#include "state.h"
#include <stdlib.h>

void caught_add_test(const char *name, void (*handler)())
{
    if (caught_internal.tests == NULL)
    {
        caught_internal.tests_capacity = CAUGHT_INTERNAL_TESTS_INITIAL_CAPACITY;
        caught_internal.tests = malloc(sizeof(caught_internal_test) * caught_internal.tests_capacity);
    }
    else if (caught_internal.tests_num == caught_internal.tests_capacity)
    {
        caught_internal.tests_capacity *= CAUGHT_INTERNAL_TESTS_CAPACITY_GROW_RATE;
        caught_internal.tests = realloc(caught_internal.tests, sizeof(caught_internal_test) * caught_internal.tests_capacity);
    }

    const caught_internal_test test = {
        .name = name,
        .handler = handler,
    };

    caught_internal.tests[caught_internal.tests_num++] = test;
}
