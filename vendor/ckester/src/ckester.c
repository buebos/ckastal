#ifndef __CKASTAL_TEST_CKASTAL_TEST_RUNNER_C__
#define __CKASTAL_TEST_CKASTAL_TEST_RUNNER_C__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test result tracking. */
typedef struct {
    int total;
    int passed;
    int failed;
    const char* current_test;
    int current_assertions;
    int verbose;
} Ckastal_TestResults;

static Ckastal_TestResults _test_results = {0, 0, 0, NULL, 0, 0};

/* Enable verbose output for detailed test information. */
#define CKASTAL_VERBOSE() (_test_results.verbose = 1)

/* Internal macro for assertions. */
#define _CKASTAL_ASSERT(condition, msg)                              \
    do {                                                             \
        _test_results.current_assertions++;                          \
        if (!(condition)) {                                          \
            _test_results.failed++;                                  \
            if (_test_results.verbose) {                             \
                fprintf(stderr, "\033[1;31m✗\033[0m %s\n", msg);     \
                fprintf(stderr, "  at %s:%d\n", __FILE__, __LINE__); \
            }                                                        \
            return;                                                  \
        } else {                                                     \
            _test_results.passed++;                                  \
            if (_test_results.verbose) {                             \
                fprintf(stdout, "\033[1;32m✓\033[0m %s\n", msg);     \
            }                                                        \
        }                                                            \
    } while (0)

/* Public assertion macros. */
#define ASSERT_TRUE(condition) _CKASTAL_ASSERT((condition), #condition " should be true")
#define ASSERT_FALSE(condition) _CKASTAL_ASSERT(!(condition), #condition " should be false")
#define ASSERT_EQ(a, b) _CKASTAL_ASSERT((a) == (b), #a " should equal " #b)
#define ASSERT_NEQ(a, b) _CKASTAL_ASSERT((a) != (b), #a " should not equal " #b)
#define ASSERT_NULL(ptr) _CKASTAL_ASSERT((ptr) == NULL, #ptr " should be NULL")
#define ASSERT_NOT_NULL(ptr) _CKASTAL_ASSERT((ptr) != NULL, #ptr " should not be NULL")
#define ASSERT_STR_EQ(a, b) _CKASTAL_ASSERT(strcmp((a), (b)) == 0, #a " should equal " #b)
#define ASSERT_STR_NEQ(a, b) _CKASTAL_ASSERT(strcmp((a), (b)) != 0, #a " should not equal " #b)

/* Test registration system. */
typedef struct {
    void (*func)(void);
    const char* name;
} Ckastal_TestEntry;

typedef struct {
    Ckastal_TestEntry* tests;
    int count;
    int capacity;
} Ckastal_TestRegistry;

static Ckastal_TestRegistry _test_registry = {NULL, 0, 0};

/* Register a test function with its name. */
static void _ckastal_register_test(void (*test)(void), const char* name) {
    if (_test_registry.count >= _test_registry.capacity) {
        _test_registry.capacity = _test_registry.capacity == 0 ? 8 : _test_registry.capacity * 2;
        _test_registry.tests = realloc(_test_registry.tests, _test_registry.capacity * sizeof(Ckastal_TestEntry));
    }
    _test_registry.tests[_test_registry.count].func = test;
    _test_registry.tests[_test_registry.count].name = name;
    _test_registry.count++;
}

/* Auto-registration helper using GCC constructor attribute */
#if defined(__GNUC__) || defined(__clang__)
#define _CKASTAL_CONSTRUCTOR __attribute__((constructor))
#elif defined(_MSC_VER)
/* MSVC uses different approach */
#define _CKASTAL_CONSTRUCTOR
#pragma section(".CRT$XCU", read)
#define _CKASTAL_MSVC_INIT(f, p)                             \
    static void f(void);                                     \
    __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
    __pragma(comment(linker, "/include:" p #f "_")) static void f(void)
#else
#define _CKASTAL_CONSTRUCTOR
#warning "Auto-registration not supported on this compiler. Please register tests manually."
#endif

/* Test definition macro with auto-registration. */
#ifdef _MSC_VER
#define TEST(test_name)                                       \
    static void test_##test_name(void);                       \
    static void _run_##test_name(void) {                      \
        _test_results.current_test = #test_name;              \
        _test_results.current_assertions = 0;                 \
        test_##test_name();                                   \
    }                                                         \
    _CKASTAL_MSVC_INIT(_init_##test_name, "") {               \
        _ckastal_register_test(_run_##test_name, #test_name); \
    }                                                         \
    static void test_##test_name(void)
#else
#define TEST(test_name)                                        \
    static void test_##test_name(void);                        \
    static void _run_##test_name(void) {                       \
        _test_results.current_test = #test_name;               \
        _test_results.current_assertions = 0;                  \
        test_##test_name();                                    \
    }                                                          \
    _CKASTAL_CONSTRUCTOR static void _init_##test_name(void) { \
        _ckastal_register_test(_run_##test_name, #test_name);  \
    }                                                          \
    static void test_##test_name(void)
#endif

/* Run all registered tests with Node.js-style output. */
static int _ckastal_run_all_tests(void) {
    int test_failures = 0;

    if (_test_results.verbose) {
        printf("\n");
    }

    for (int i = 0; i < _test_registry.count; i++) {
        int before_failed = _test_results.failed;

        if (_test_results.verbose) {
            printf("\033[1;36m▶\033[0m %s\n", _test_registry.tests[i].name);
        }

        _test_registry.tests[i].func();

        if (_test_results.failed > before_failed) {
            test_failures++;
            if (!_test_results.verbose) {
                fprintf(stderr, "\033[1;31m✗\033[0m %s\n", _test_registry.tests[i].name);
            }
        } else {
            if (!_test_results.verbose) {
                printf("\033[1;32m✓\033[0m %s\n", _test_registry.tests[i].name);
            }
        }

        if (_test_results.verbose) {
            printf("\n");
        }
    }

    printf("\n");

    if (test_failures == 0) {
        printf("[INFO]:  All tests passed\n");
        if (_test_results.verbose) {
            printf("         %d tests, %d assertions\n", _test_registry.count, _test_results.passed);
        }
        free(_test_registry.tests);
        return 0;
    } else {
        printf("\033[1;31m[FAIL]:\033[0m %d/%d tests failed\n", test_failures, _test_registry.count);
        if (_test_results.verbose) {
            printf("         %d assertions passed, %d failed\n",
                   _test_results.passed, _test_results.failed);
        }
        free(_test_registry.tests);
        return 1;
    }
}

/* Auto-generate main function that runs all tests. */
int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            CKASTAL_VERBOSE();
            break;
        }
    }
    return _ckastal_run_all_tests();
}

#endif
