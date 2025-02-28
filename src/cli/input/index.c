#ifndef __CKASTAL_INPUT_C__
#define __CKASTAL_INPUT_C__

#include <stdio.h>
#include <stdlib.h>

/** */
#include "../../core/init.c"
#include "../../core/os.c"
#include "../../dsa/string/null_terminated/index.c"

#ifdef CKASTAL_CORE_OS_WINDOWS
#include "../../vendor/gnu-win-regex-2.7-src/src/regex.c"
#else
#include <regex.h>
#endif

#ifdef CKASTAL_CORE_OS_WINDOWS
#define CKASTAL_INPUT_STDIN_BACKSPACE 8
#elif defined(CKASTAL_CORE_OS_IS_UNIX_BASED)
#define CKASTAL_INPUT_STDIN_BACKSPACE 127
#endif
#define CKASTAL_INPUT_STDIN_ENTER 13

typedef enum Ck_ValidationStatus {
    CK_VALIDATION_OK = 0,
    CK_VALIDATION_ERROR,
    CK_VALIDATION_WARN,
} Ck_ValidationStatus;

typedef struct Ck_ValidationRes {
    Ck_ValidationStatus status;
    char* message;
} Ck_ValidationRes;

typedef struct Ck_InputParams {
    char* prompt;

    Ck_ValidationRes (*validator)(char*);

    bool use_max_length;
    size_t max_length;

    void (*on_before_prompt)();
    void (*on_after_prompt)();
} Ck_InputParams;

char _ck_getch(void);

#if defined(CKASTAL_CORE_OS_MACOS) || defined(CKASTAL_CORE_OS_LINUX)
#include <termios.h>
#include <unistd.h>

char _ck_getch(void) {
    char buf = 0;
    struct termios old = {0};

    fflush(stdout);

    if (tcgetattr(0, &old) < 0) {
        perror("tcsetattr()");
    }

    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &old) < 0) {
        perror("tcsetattr ICANON");
    }

    if (read(0, &buf, 1) < 0) {
        perror("read()");
    }

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    if (tcsetattr(0, TCSADRAIN, &old) < 0) {
        perror("tcsetattr ~ICANON");
    }

    return buf;
}
#elif defined(CKASTAL_CORE_OS_WINDOWS)
#include <conio.h>

char _ck_getch(void) {
    return getch();
}
#endif

void ck_input(char* buffer, size_t buffer_size, Ck_InputParams params) {
    if (NULL != params.prompt) {
        printf("%s", params.prompt);
    }

    Ck_ValidationRes res = {.status = CK_VALIDATION_OK, .message = NULL};

    size_t cursor = 0;
    char c = _ck_getch();

    if (params.validator) {
        res = params.validator(buffer);
    }

    while ((c != '\n' && c != CKASTAL_INPUT_STDIN_ENTER) || res.status != CK_VALIDATION_OK) {
        if (c == CKASTAL_INPUT_STDIN_BACKSPACE) {
            if (cursor > 0) {
                buffer[--cursor] = 0;
                /** Deletes the char from stdin */
                printf("\b \b");
            }

            c = _ck_getch();
            continue;
        }
        /**
         * Substract one to save space for the null terminator. This will
         * continue reading until the cursor is decreased via input of
         * backspace or the new line char is read and the input is submitted.
         */
        if (
            cursor == buffer_size - 1 ||
            (params.use_max_length && cursor == params.max_length - 1)

        ) {
            c = _ck_getch();
            continue;
        }

        buffer[cursor] = c;

        if (params.validator) {
            res = params.validator(buffer);

            if (res.status == CK_VALIDATION_ERROR) {
                buffer[cursor] = 0;
                c = _ck_getch();
                continue;
            }
        }

        printf("%c", c);
        cursor++;

        c = _ck_getch();
    }

    printf("\n");
}

Ck_ValidationRes _ck_input_yes_or_no_validator(char* input) {
    Ck_ValidationRes res = {
        .status = CK_VALIDATION_ERROR,
        .message = NULL,
    };

    if (input[0] == 'y' && input[1] == 0 || input[0] == 'n' && input[1] == 0) {
        res.status = CK_VALIDATION_OK;
    }

    return res;
}

bool ck_input_yes_or_no(char* prompt) {
    char buffer[2] = {0};
    ck_input(buffer, 2, (Ck_InputParams){.prompt = prompt, .validator = _ck_input_yes_or_no_validator, 0});
    return buffer[0] == 'y';
}

Ck_ValidationRes _ck_input_float_validator(char* value) {
    Ck_ValidationRes result = {
        .status = CK_VALIDATION_OK,
        .message = "Success",
    };

    regex_t valid_regex, incomplete_regex;

    const char* valid_pattern = "^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?$";
    const char* incomplete_pattern = "^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)[eE][+-]?$";

    if (regcomp(&valid_regex, valid_pattern, REG_EXTENDED) ||
        regcomp(&incomplete_regex, incomplete_pattern, REG_EXTENDED)) {
        result.status = CK_VALIDATION_ERROR;
        result.message = "Regex compilation error";
        return result;
    }

    if (regexec(&valid_regex, value, 0, NULL, 0) == 0) {
        result.status = CK_VALIDATION_OK;
        result.message = "Success";
    } else if (regexec(&incomplete_regex, value, 0, NULL, 0) == 0) {
        result.status = CK_VALIDATION_WARN;
        result.message = "Warning: Incomplete float";
    } else {
        result.status = CK_VALIDATION_ERROR;
        result.message = "Error: Invalid float";
    }

    regfree(&valid_regex);
    regfree(&incomplete_regex);

    return result;
}

float ck_input_float(char* prompt) {
    /**
     * To get a correct precision in the parsing
     */
    const char BUFFER_FLOAT_SIZE = 26;
    char buffer[BUFFER_FLOAT_SIZE] = {0};

    ck_input(
        buffer,
        BUFFER_FLOAT_SIZE,
        (Ck_InputParams){
            .prompt = prompt,
            .validator = _ck_input_float_validator,
            0,
        }

    );

    return atof(buffer);
}

void ck_input_to_continue(char* prompt) {
    if (NULL != prompt) {
        printf("%s", prompt);
    }

    char c = _ck_getch();

    while ((c != '\n' && c != CKASTAL_INPUT_STDIN_ENTER)) {
        c = _ck_getch();
    }

    printf("\n");
}

#endif /* __CKASTAL_INPUT_C__ */
