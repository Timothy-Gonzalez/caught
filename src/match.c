#include "match.h"
#include "output.h"

#include <assert.h>
#include <stdlib.h>

static caught_internal_matcher MATCHERS[] = {
    {
        .specifiers = "id",
        .whitelist = "0123456789",
        .blacklist = NULL,
        .match_all = true,
        .only_one = '\0',
    },
    {
        .specifiers = "f",
        .whitelist = "0123456789.",
        .blacklist = NULL,
        .only_one = '.',
        .match_all = true,
    },
    {
        .specifiers = "c",
        .whitelist = NULL,
        .blacklist = NULL,
        .only_one = '\0',
        .match_all = false,
    },
    {
        .specifiers = "a",
        .whitelist = "abcdefghijklmnopqrstuvwxyABDEFGHIKLMNOPQRSWTUVWXYZ",
        .blacklist = NULL,
        .only_one = '\0',
        .match_all = true,
    },
    {
        .specifiers = "w",
        .whitelist = NULL,
        .blacklist = "\n\r\v\f\t ",
        .only_one = '\0',
        .match_all = true,
    },
    {
        .specifiers = "s",
        .whitelist = NULL,
        .blacklist = NULL,
        .only_one = '\0',
        .match_all = true,
    },
};

static caught_internal_matcher get_matcher(char specifier)
{
    size_t i;
    for (i = 0; i < sizeof(MATCHERS) / sizeof(MATCHERS[0]); i++)
    {
        size_t j;
        char *specifiers = MATCHERS[i].specifiers;
        for (j = 0; specifiers[j] != '\0'; ++j)
        {
            if (specifier == specifiers[j])
            {
                return MATCHERS[i];
            }
        }
    }

    caught_output_errorf("Invalid format specifier: $%c", specifier);
    return (caught_internal_matcher){};
}

static caught_internal_matcher_fstr_segment *break_fstr_segments(char *fstr)
{
    size_t length = strlen(fstr);

    size_t parts = 1;

    size_t i;
    for (i = 0; i < length; ++i)
    {
        if (fstr[i] == '$')
        {
            parts += 2; // Technically wrong for $$, but still a good enough heuristic
        }
    }

    caught_internal_matcher_fstr_segment *result = malloc(sizeof(caught_internal_matcher_fstr_segment) * (parts + 1));
    size_t result_size = 0;
    caught_internal_matcher_fstr_segment segment;

    char *buffer = malloc(sizeof(char) * (length + 1));
    buffer[0] = '\0';
    int buffer_i = 0;
    for (i = 0; i <= length; ++i)
    {
        // if end of string OR found delimeter
        bool is_$ = (i < length) && (fstr[i] == '$');
        bool next_$ = (i < length - 1) && (fstr[i + 1] == '$');
        bool is_end_of_specifier = (strlen(buffer) >= 2) && (segment.is_specifier);
        bool split = (is_$ && !next_$) || (is_end_of_specifier);

        if ((i == length || split) && strlen(buffer) >= 1)
        {
            segment.segment = strdup(buffer);
            if (segment.is_specifier)
            {
                segment.matcher = get_matcher(buffer[1]);
            }
            result[result_size++] = segment;
            segment = (caught_internal_matcher_fstr_segment){};
            buffer[0] = '\0';
            buffer_i = 0;
        }

        if (i == length)
        {
            break;
        }

        if (buffer_i == 0)
        {
            segment.is_specifier = is_$ && !next_$;
        }

        buffer[buffer_i] = fstr[i];
        buffer[buffer_i + 1] = '\0';
        buffer_i++;
        if (is_$ && next_$)
        {
            i++;
        }
    }

    free(buffer);

    result[result_size] = (caught_internal_matcher_fstr_segment){
        .segment = NULL,
    };

    return result;
}

static void break_fstr_segments_cleanup(caught_internal_matcher_fstr_segment *segments)
{
    size_t i;
    for (i = 0; segments[i].segment != NULL; ++i)
    {
        free(segments[i].segment);
    }
    free(segments);
}

static bool match(char *str, caught_internal_matcher_fstr_segment *segments)
{
    caught_internal_matcher_fstr_segment segment = *segments;
    char *segment_str = segment.segment;
    if (segment_str == NULL)
    {
        // we reached the last segment, we did it!
        return true;
    }
    caught_internal_matcher matcher = segment.matcher;

    caught_internal_matcher_fstr_segment next_segment = *(segments + 1);
    char *next_segment_str = next_segment.segment;
    bool last_segment = next_segment_str == NULL;

    // Literal matches
    if (!segment.is_specifier)
    {
        if (strncmp(str, segment_str, strlen(segment_str)) != 0)
        {
            return false;
        }
        return match(str + strlen(segment_str), segments + 1);
    }

    // Specifier matches
    bool found_match = false;

    size_t match_size = 0;
    bool only_one_flag = false;
    while (match_size < strlen(str))
    {
        char c = str[match_size];

        // Apply whitelist
        if (matcher.whitelist)
        {
            if (strchr(matcher.whitelist, c) == NULL)
            {
                break;
            }
        }

        // Apply blacklist
        if (matcher.blacklist)
        {
            if (strchr(matcher.blacklist, c) != NULL)
            {
                break;
            }
        }

        // Only one condition - if more than one we fail
        if (matcher.only_one != '\0' && c == matcher.only_one)
        {
            if (only_one_flag)
            {
                break;
            }
            only_one_flag = true;
        }

        match_size += 1;

        // Look ahead - if next non-specifier doesn't match - we can't do this
        if (!last_segment && !next_segment.is_specifier)
        {
            if (strncmp(str + match_size, next_segment_str, strlen(next_segment_str)) != 0)
            {
                continue;
            }
        }

        // Match size worked
        found_match = match(str + match_size, segments + 1);

        if (found_match)
        {
            break;
        }

        if (match_size == 1 && !matcher.match_all)
        {
            break;
        }
    }

    return found_match;
}

bool caught_internal_match(char *str, char *fstr)
{
    assert(str);
    assert(fstr);

    caught_internal_matcher_fstr_segment *segments = break_fstr_segments(fstr);
    size_t segments_len = 0;
    while (segments[segments_len].segment != NULL)
    {
        segments_len++;
    }

    bool result = match(str, segments);
    break_fstr_segments_cleanup(segments);
    return result;
}
