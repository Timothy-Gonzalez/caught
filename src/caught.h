// The central entrypoint of caught
// You should include this file from your tests
// #include "caught.h"

#define CAUGHT_VERSION_MAJOR 0
#define CAUGHT_VERSION_MINOR 2
#define CAUGHT_VERSION_PATCH 2
#define CAUGHT_VERSION_STRING "v0.2.2"

#define _GNU_SOURCE                                     // needed to make compiler happy, since this is the entrypoint
extern int asprintf(char **strp, const char *fmt, ...); // sometimes also needed

#ifndef CAUGHT_LIB
#define CAUGHT_LIB

#include "assertion-macros.h"
#include "mocks.h"
#include "tests.h"

#endif
