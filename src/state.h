#include <stddef.h>
#include "tests.h"
#ifndef CAUGHT_STATE
#define CAUGHT_STATE

#define CAUGHT_INTERNAL_TESTS_INITIAL_CAPACITY 8;
#define CAUGHT_INTERNAL_TESTS_CAPACITY_GROW_RATE 2;

struct caught_internal_t
{
    int assertions, passed_assertions;
    caught_internal_test *tests;
    int tests_num;
    int tests_capacity;
};

extern struct caught_internal_t caught_internal_state;

#endif
