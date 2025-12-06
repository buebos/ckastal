/*
 * Ckester - C Kit Test Runner CLI
 * Consolidates test execution, compilation, and management into a single binary.
 */

#ifndef __CKASTAL_TEST_RUNNER_CLI_C__
#define __CKASTAL_TEST_RUNNER_CLI_C__

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* ANSI color codes for output formatting. */
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD "\033[1m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN "\033[1;36m"

/* Configuration constants. */
#define TEST_DIR "./test/suite"
#define DEFAULT_BUILD_DIR "./build/ckester"
#define MAX_PATH 512
#define MAX_COMMAND 2048

/* Test suite levels in priority order for search. */
static const char* TEST_SUITE_LEVELS[] = {
    "UNIT",
    "INTEGRATION",
    "SYSTEM",
    NULL};

static const char* TEST_SUITE_LEVELS_LOWER[] = {
    "unit",
    "integration",
    "system",
    NULL};

/* Execution modes. */
typedef enum {
    MODE_DEFAULT,
    MODE_VERBOSE,
    MODE_SILENT
} Mode;

/* Runner configuration. */
typedef struct {
    Mode mode;
    char cflags[256];
    char cppflags[256];
    char build_dir[MAX_PATH];
    const char* test_name;
    const char* test_level;
} RunnerConfig;

/* Utility function to check if a file exists. */
static int _file_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0;
}

/* Utility function to check if file A is newer than file B. */
static int _file_is_newer(const char* file_a, const char* file_b) {
    struct stat st_a, st_b;
    if (stat(file_a, &st_a) != 0 || stat(file_b, &st_b) != 0) {
        return 0;
    }
    return st_a.st_mtime > st_b.st_mtime;
}

/* Create directory recursively. */
static int _mkdir_recursive(const char* path) {
    char tmp[MAX_PATH];
    char* p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
                return -1;
            }
            *p = '/';
        }
    }
    if (mkdir(tmp, 0755) != 0 && errno != EEXIST) {
        return -1;
    }
    return 0;
}

/* Compile a test file. */
static int _compile_test(const char* src_path, const char* bin_path, RunnerConfig* cfg) {
    char command[MAX_COMMAND];

    printf(COLOR_MAGENTA "[BUILD]" COLOR_RESET " Compiling %s\n", src_path);

    snprintf(command, sizeof(command), "cc -Wall %s %s %s -o %s",
             cfg->cflags, cfg->cppflags, src_path, bin_path);

    int result = system(command);
    return result == 0 ? 0 : -1;
}

/* Execute a single test binary. */
static int _execute_test(const char* bin_path, const char* src_path, RunnerConfig* cfg) {
    char command[MAX_COMMAND];
    char output[32768];
    FILE* fp;
    int passed = 1;

    if (cfg->mode != MODE_SILENT) {
        char abs_bin[MAX_PATH];
        char abs_src[MAX_PATH];
        realpath(bin_path, abs_bin);
        realpath(src_path, abs_src);

        printf(COLOR_CYAN "[BIN]" COLOR_RESET " %s\n", abs_bin);
        printf(COLOR_CYAN "[SRC]" COLOR_RESET " %s\n", abs_src);
    }

    /* Build command with verbose flag if needed. */
    if (cfg->mode == MODE_VERBOSE) {
        snprintf(command, sizeof(command), "%s --verbose 2>&1", bin_path);
    } else {
        snprintf(command, sizeof(command), "%s 2>&1", bin_path);
    }

    fp = popen(command, "r");
    if (!fp) {
        fprintf(stderr, COLOR_RED "[ERROR]" COLOR_RESET " Failed to execute test\n");
        return -1;
    }

    /* Capture and display output. */
    output[0] = '\0';
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        strcat(output, line);
        if (cfg->mode != MODE_SILENT || strstr(line, "tests, ") != NULL) {
            printf("  %s", line);
        }
    }

    int status = pclose(fp);
    passed = (status == 0 && strstr(output, "All tests passed") != NULL);

    if (cfg->mode != MODE_SILENT) {
        if (passed) {
            printf(COLOR_GREEN "  ✓ Passed" COLOR_RESET "\n\n");
        } else {
            printf(COLOR_RED "  ✗ Failed" COLOR_RESET "\n\n");
        }
    }

    return passed ? 0 : -1;
}

/* Run all tests in a specific suite level. */
static int _run_level_tests(const char* test_level, RunnerConfig* cfg, int* total, int* passed, int* failed) {
    glob_t globbuf;
    char pattern[MAX_PATH];
    char build_suite_dir[MAX_PATH];

    snprintf(pattern, sizeof(pattern), "%s/%s/*.c", TEST_DIR, test_level);
    snprintf(build_suite_dir, sizeof(build_suite_dir), "%s/%s", cfg->build_dir, test_level);

    /* Check if suite directory exists. */
    if (glob(pattern, 0, NULL, &globbuf) != 0) {
        return 0;
    }

    if (globbuf.gl_pathc == 0) {
        globfree(&globbuf);
        return 0;
    }

    /* Ensure build directory exists for this suite. */
    _mkdir_recursive(build_suite_dir);

    /* Compile and run each test. */
    for (size_t i = 0; i < globbuf.gl_pathc; i++) {
        (*total)++;

        const char* src_path = globbuf.gl_pathv[i];

        /* Extract test name from path. */
        const char* filename = strrchr(src_path, '/');
        filename = filename ? filename + 1 : src_path;

        char test_name[256];
        strncpy(test_name, filename, sizeof(test_name));
        test_name[strlen(test_name) - 2] = '\0'; /* Remove .c extension. */

        /* Build binary path. */
        char bin_path[MAX_PATH];
        snprintf(bin_path, sizeof(bin_path), "%s/%s", build_suite_dir, test_name);

        /* Compile if needed. */
        if (!_file_exists(bin_path) || _file_is_newer(src_path, bin_path)) {
            if (_compile_test(src_path, bin_path, cfg) != 0) {
                (*failed)++;
                continue;
            }
        }

        /* Execute test. */
        if (_execute_test(bin_path, src_path, cfg) == 0) {
            (*passed)++;
        } else {
            (*failed)++;
        }
    }

    globfree(&globbuf);
    return globbuf.gl_pathc;
}

/* Run tests for specific suite level or all. */
static int _run_tests(RunnerConfig* cfg) {
    int total = 0;
    int passed = 0;
    int failed = 0;

    printf("\n" COLOR_BLUE "========================================\n");
    printf("   Ckester - C Kit Test Runner\n");
    printf("========================================" COLOR_RESET "\n\n");

    printf(COLOR_MAGENTA "[MODE]: ");
    switch (cfg->mode) {
        case MODE_VERBOSE:
            printf("VERBOSE");
            break;
        case MODE_SILENT:
            printf("SILENT");
            break;
        default:
            printf("DEFAULT");
            break;
    }
    printf(COLOR_RESET "\n");

    if (strcmp(cfg->test_level, "all") == 0) {
        printf(COLOR_BLUE "[LEVEL]: ");

        size_t i = 0;

        while (TEST_SUITE_LEVELS[i] != NULL) {
            printf("%s", TEST_SUITE_LEVELS[i]);

            if (TEST_SUITE_LEVELS[i + 1] != NULL) {
                printf(", ");
            }

            i++;
        }

        printf(COLOR_RESET "\n");
    } else {
        printf(COLOR_BLUE "[LEVEL]: ");
        for (int i = 0; i < strlen(cfg->test_level); i++) {
            printf("%c", toupper(cfg->test_level[i]));
        }
        printf(COLOR_RESET "\n");
    }

    printf("\n");

    /* Ensure build directory exists. */
    _mkdir_recursive(cfg->build_dir);

    /* Run tests for specific suite level or all. */
    if (strcasecmp(cfg->test_level, "all") != 0) {
        /* Run specific suite level. */
        int found = 0;
        for (int i = 0; TEST_SUITE_LEVELS[i] != NULL; i++) {
            if (strcasecmp(TEST_SUITE_LEVELS[i], cfg->test_level) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            fprintf(stderr, COLOR_RED "[ERROR]" COLOR_RESET " Unknown test level: %s\n", cfg->test_level);
            fprintf(stderr, "Available levels: ");
            for (int i = 0; TEST_SUITE_LEVELS[i] != NULL; i++) {
                fprintf(stderr, "%s%s", i > 0 ? ", " : "", TEST_SUITE_LEVELS[i]);
            }
            fprintf(stderr, "\n");
            return -1;
        }

        _run_level_tests(cfg->test_level, cfg, &total, &passed, &failed);
    } else {
        /* Run all test levels. */
        for (int i = 0; TEST_SUITE_LEVELS_LOWER[i] != NULL; i++) {
            _run_level_tests(TEST_SUITE_LEVELS_LOWER[i], cfg, &total, &passed, &failed);
        }
    }

    if (total == 0) {
        fprintf(stderr, COLOR_YELLOW "[WARNING]" COLOR_RESET " No test files found\n");
        return 0;
    }

    /* Print summary. */
    printf("\n");
    if (failed == 0) {
        printf(COLOR_GREEN "  ✓ %d passed, %d total" COLOR_RESET "\n", passed, total);
        return 0;
    } else {
        printf(COLOR_RED "  ✗ %d failed, %d total" COLOR_RESET "\n", failed, total);
        return 1;
    }
}

/* Find test file by searching through suite types in priority order. */
static int _find_test_file(const char* test_name, char* src_path, char* test_level) {
    for (int i = 0; TEST_SUITE_LEVELS[i] != NULL; i++) {
        snprintf(src_path, MAX_PATH, "%s/%s/%s.c", TEST_DIR, TEST_SUITE_LEVELS[i], test_name);
        if (_file_exists(src_path)) {
            strcpy(test_level, TEST_SUITE_LEVELS[i]);
            return 0;
        }
    }
    return -1;
}

/* Run a single test. */
static int _run_single_test(RunnerConfig* cfg) {
    if (!cfg->test_name) {
        fprintf(stderr, COLOR_RED "[ERROR]" COLOR_RESET " Please specify test name\n");
        fprintf(stderr, "Usage: ckester single <test_name>\n");
        return -1;
    }

    char src_path[MAX_PATH];
    char test_level[64];
    char bin_path[MAX_PATH];
    char build_suite_dir[MAX_PATH];

    /* Find test file across suite types. */
    if (_find_test_file(cfg->test_name, src_path, test_level) != 0) {
        fprintf(stderr, COLOR_RED "[ERROR]" COLOR_RESET " Test file '%s.c' not found in any suite\n", cfg->test_name);
        fprintf(stderr, "Searched in: ");
        for (int i = 0; TEST_SUITE_LEVELS[i] != NULL; i++) {
            fprintf(stderr, "%s%s", i > 0 ? ", " : "", TEST_SUITE_LEVELS[i]);
        }
        fprintf(stderr, "\n");
        return -1;
    }

    /* Build paths. */
    snprintf(build_suite_dir, sizeof(build_suite_dir), "%s/%s", cfg->build_dir, test_level);
    snprintf(bin_path, sizeof(bin_path), "%s/%s", build_suite_dir, cfg->test_name);

    /* Ensure build directory exists. */
    _mkdir_recursive(build_suite_dir);

    /* Compile if needed. */
    if (!_file_exists(bin_path) || _file_is_newer(src_path, bin_path)) {
        if (_compile_test(src_path, bin_path, cfg) != 0) {
            return -1;
        }
    }

    printf("\n" COLOR_BLUE "========================================\n");
    printf("   Ckester - C Kit Test Runner\n");
    printf("========================================" COLOR_RESET "\n\n");

    printf("[MODE]: ");
    switch (cfg->mode) {
        case MODE_VERBOSE:
            printf("VERBOSE");
            break;
        case MODE_SILENT:
            printf("SILENT");
            break;
        default:
            printf("DEFAULT");
            break;
    }
    printf("\n");

    /* Execute test. */
    int result = _execute_test(bin_path, src_path, cfg);

    if (cfg->mode == MODE_SILENT) {
        if (result == 0) {
            printf(COLOR_GREEN "✓ Passed" COLOR_RESET "\n");
        } else {
            printf(COLOR_RED "✗ Failed" COLOR_RESET "\n");
        }
    }

    return result;
}

/* Clean build artifacts. */
static int _clean_build(RunnerConfig* cfg) {
    char command[MAX_COMMAND];
    printf("Cleaning build artifacts in %s...\n", cfg->build_dir);
    snprintf(command, sizeof(command), "rm -rf %s", cfg->build_dir);
    int result = system(command);
    if (result == 0) {
        printf(COLOR_GREEN "✓ Build directory cleaned" COLOR_RESET "\n");
    } else {
        fprintf(stderr, COLOR_RED "✗ Failed to clean build directory" COLOR_RESET "\n");
    }
    return result;
}

/* Display help information. */
static void _print_help(void) {
    printf("\n" COLOR_BLUE "Ckester - C Kit Test Runner" COLOR_RESET "\n\n");
    printf("Usage: ckester <command> [options]\n\n");
    printf(COLOR_BLUE "Commands:" COLOR_RESET "\n");
    printf("  " COLOR_GREEN "all" COLOR_RESET "              Run all tests across all suites (unit, integration, system)\n");
    printf("  " COLOR_GREEN "single" COLOR_RESET " <name>    Run a single test (searches unit, integration, system in order)\n");
    printf("  " COLOR_GREEN "clean" COLOR_RESET "            Clean build artifacts\n");
    printf("  " COLOR_GREEN "help" COLOR_RESET "             Show this help message\n\n");
    printf(COLOR_BLUE "Options:" COLOR_RESET "\n");
    printf("  " COLOR_YELLOW "-v, --verbose" COLOR_RESET "   Enable verbose output\n");
    printf("  " COLOR_YELLOW "-s, --silent" COLOR_RESET "    Enable silent mode\n");
    printf("  " COLOR_YELLOW "-d, --default" COLOR_RESET "   Use default mode (default)\n");
    printf("  " COLOR_YELLOW "-b, --build-dir" COLOR_RESET " <path>  Set build directory (default: ./build/ckastal_test)\n\n");
    printf(COLOR_BLUE "Examples:" COLOR_RESET "\n");
    printf("  ckester all\n");
    printf("  ckester all --verbose\n");
    printf("  ckester all --build-dir ./custom_build\n");
    printf("  ckester single array_test\n");
    printf("  ckester single lenstr_test --verbose\n");
    printf("  ckester clean\n\n");
}

/* Main entry point. */
int main(int argc, char* argv[]) {
    RunnerConfig cfg = {
        .mode = MODE_DEFAULT,
        .cflags = "",
        .cppflags = "",
        .build_dir = DEFAULT_BUILD_DIR,
        .test_name = NULL,
        .test_level = TEST_SUITE_LEVELS_LOWER[0],

    };

    /* Load CFLAGS and CPPFLAGS from environment. */
    const char* env_cflags = getenv("CFLAGS");
    const char* env_cppflags = getenv("CPPFLAGS");
    if (env_cflags) {
        strncpy(cfg.cflags, env_cflags, sizeof(cfg.cflags) - 1);
    }
    if (env_cppflags) {
        strncpy(cfg.cppflags, env_cppflags, sizeof(cfg.cppflags) - 1);
    }

    if (argc < 2) {
        _print_help();
        return 1;
    }

    /* Parse command and options. */
    const char* command = argv[1];

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            cfg.mode = MODE_VERBOSE;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--silent") == 0) {
            cfg.mode = MODE_SILENT;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--default") == 0) {
            cfg.mode = MODE_DEFAULT;
        } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--build-dir") == 0) {
            if (i + 1 < argc) {
                strncpy(cfg.build_dir, argv[++i], sizeof(cfg.build_dir) - 1);
            } else {
                fprintf(stderr, COLOR_RED "[ERROR]" COLOR_RESET " --build-dir requires a path argument\n");
                return 1;
            }
        } else if (!cfg.test_name) {
            cfg.test_name = argv[i];
        }
    }

    /* Execute command. */
    if (strcmp(command, "all") == 0) {
        cfg.test_level = "all";
        return _run_tests(&cfg);
    } else if (strcmp(command, "unit") == 0) {
        cfg.test_level = "unit";
        return _run_tests(&cfg);
    } else if (strcmp(command, "integration") == 0) {
        cfg.test_level = "integration";
        return _run_tests(&cfg);
    } else if (strcmp(command, "system") == 0) {
        cfg.test_level = "system";
        return _run_tests(&cfg);
    } else if (strcmp(command, "single") == 0) {
        return _run_single_test(&cfg);
    } else if (strcmp(command, "clean") == 0) {
        return _clean_build(&cfg);
    } else if (strcmp(command, "help") == 0 || strcmp(command, "--help") == 0) {
        _print_help();
        return 0;
    } else {
        fprintf(stderr, COLOR_RED "[ERROR]" COLOR_RESET " Unknown command: %s\n", command);
        _print_help();
        return 1;
    }
}

#endif
