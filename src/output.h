#include <stdlib.h>
#include <string.h>
#include "assertions.h"
#ifndef CAUGHT_OUTPUT
#define CAUGHT_OUTPUT

void caught_internal_initialize_color_output();

void caught_output_success();
void caught_output_fail();
void caught_output_warning();
void caught_output_info();
void caught_output_bold();
void caught_output_reset();

void caught_output_header();
void caught_output_status_tag(int pass);
void caught_output_assertion(caught_internal_assertion *assertion);
void caught_output_test_summary(const char *test_name, int passed, int failed);
void caught_output_generic_summary(const char *prefix, int passed, int failed);
void caught_output_summary(const char *prefix, int passed, int failed);
void caught_output_overall_result(int pass);

extern int caught_color_enabled;

#endif
