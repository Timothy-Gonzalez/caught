#include <stdbool.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>

#ifndef CAUGHT_MATCH
#define CAUGHT_MATCH

struct caught_internal_matcher
{
    char *specifiers; // Specifiers for this matcher
    char *whitelist;  // Whitelist characters
    char *blacklist;  // Blacklist characters
    char only_one;    // Character that can only be seen once, '\0' if not set
    bool match_all;   // Only match one if false
};

typedef struct caught_internal_matcher caught_internal_matcher;

struct caught_internal_matcher_fstr_segment
{
    bool is_specifier;
    caught_internal_matcher matcher;
    char *segment;
};
typedef struct caught_internal_matcher_fstr_segment caught_internal_matcher_fstr_segment;

/*
 * Returns if lhs matches rhs
 *
 * Match specifiers are specified with $
 *
 * $i, $d = match a integer (0-9)
 * $c = match a single character
 * $f = match a float (0-9, a single . allowed)
 * $a = match alpha-chars (a-z and A-Z)
 * $w = match a word (matches anything non-whitespace)
 * $s = match a string (at least 1 character)
 * $$ = a literal $
 *
 * Anything else = literal match
 *
 * Having two greedy operators like $s without a literal match between them ("$s$s") will fail
 */
bool caught_internal_match(char *str, char *fstr);

#endif
