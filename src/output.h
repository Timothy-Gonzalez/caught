#include <stdlib.h>
#include <string.h>
#include "assertion-result.h"
#ifndef CAUGHT_OUTPUT
#define CAUGHT_OUTPUT

void caught_internal_initialize_color_output();

void caught_output_success();
void caught_output_background_success();
void caught_output_fail();
void caught_output_background_fail();
void caught_output_warning();
void caught_output_info();
void caught_output_bold();
void caught_output_reset();

void caught_output_internal_error(bool use_perror, char *fstr, va_list args);
void caught_output_perrorf(char *fstr, ...);
void caught_output_errorf(char *fstr, ...);
void caught_output_printf(char *fstr, ...);

void caught_output_header();
void caught_output_status_tag(int pass);
void caught_output_assertion_result(caught_internal_assertion_result assertion_result);
void caught_output_test_summary(const char *test_name, int passed, int failed);
void caught_output_generic_summary(const char *prefix, int passed, int failed);
void caught_output_summary(const char *prefix, int passed, int failed);
void caught_output_overall_result(int pass);

extern int caught_color_enabled;

#endif
