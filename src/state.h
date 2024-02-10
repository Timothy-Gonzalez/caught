#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

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
    int mocked_stdout_pipe[2];
    int original_stdout;
    FILE *original_stdout_file;
    bool is_parent;
};

extern struct caught_internal_t caught_internal_state;

void caught_internal_cleanup_state(bool exiting);

#endif
