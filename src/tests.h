#ifndef CAUGHT_TESTS
#define CAUGHT_TESTS

typedef struct caught_internal_test
{
    const char *name;
    void (*handler)(void);
} caught_internal_test;

void caught_add_test(const char *name, void (*handler)());

#define CAUGHT_INTERNAL_STRING(a) #a
#define CAUGHT_INTERNAL_CONCAT(a, b) a##b
#define CAUGHT_INTERNAL_EXPAND_AND_CONCAT(a, b) CAUGHT_INTERNAL_CONCAT(a, b)
#define CAUGHT_INTERNAL_TEST_UNIQUE_NAME(base) CAUGHT_INTERNAL_EXPAND_AND_CONCAT(base, __LINE__)

#define TEST(name)                                                                            \
    static void CAUGHT_INTERNAL_TEST_UNIQUE_NAME(test_line_)();                               \
    __attribute__((constructor)) static void CAUGHT_INTERNAL_TEST_UNIQUE_NAME(__test_case_)() \
    {                                                                                         \
        caught_add_test(name, CAUGHT_INTERNAL_TEST_UNIQUE_NAME(test_line_));                  \
    }                                                                                         \
    static void CAUGHT_INTERNAL_TEST_UNIQUE_NAME(test_line_)()

#endif
