#include "state.h"

struct caught_internal_t caught_internal_state = {
    .assertions = 0,
    .passed_assertions = 0,
    .tests = NULL,
    .tests_num = 0,
    .tests_capacity = 0,
};
