#ifndef __CKASTAL_INPUT_C__
#define __CKASTAL_INPUT_C__

#include <stdio.h>
/** */
#include "../core/init.c"
#include "../core/os.c"
#include "../validation/index.c"

#define CKASTAL_INPUT_STDIN_BACKSPACE 127

typedef struct Ck_InputParams {
    char* prompt;

    Ck_ValidationRes (*validator)(char*);

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
#elif CKASTAL_CORE_OS_WINDOWS
#include <conio.h>

char _ck_getch(void) {
    return getch();
}
#endif

void ck_input(char* buffer, size_t buffer_size, Ck_InputParams params) {
    if (NULL != params.prompt) {
        printf("%s", params.prompt);
    }

    size_t cursor = 0;
    char c = _ck_getch();

    while (c != '\n') {
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
        if (cursor == buffer_size - 1) {
            c = _ck_getch();
            continue;
        }

        buffer[cursor] = c;

        if (params.validator) {
            Ck_ValidationRes res = params.validator(buffer);

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

#endif /* __CKASTAL_INPUT_C__ */
