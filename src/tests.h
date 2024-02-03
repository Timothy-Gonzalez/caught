#ifndef CAUGHT_TESTS
#define CAUGHT_TESTS

typedef struct caught_internal_test
{
    const char *name;
    void (*handler)(void);
} caught_internal_test;

void caught_add_test(const char *name, void (*handler)());

#define STRING(a) #a
#define CONCAT(a, b) a##b
#define EXPAND_AND_CONCAT(a, b) CONCAT(a, b)
#define CAUGHT_TEST_UNIQUE_NAME(base) EXPAND_AND_CONCAT(base, __LINE__)

#define TEST(name)                                                                   \
    static void CAUGHT_TEST_UNIQUE_NAME(test_)();                                    \
    __attribute__((constructor)) static void CAUGHT_TEST_UNIQUE_NAME(__test_case_)() \
    {                                                                                \
        caught_add_test(name, CAUGHT_TEST_UNIQUE_NAME(test_));                       \
    }                                                                                \
    static void CAUGHT_TEST_UNIQUE_NAME(test_)()

#endif
