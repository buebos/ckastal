#ifndef __CKASTAL_INPUT_C__
#define __CKASTAL_INPUT_C__

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/** */
#include "../../core/init.c"
#include "../validation/index.c"

typedef struct Ck_InputParams {
    char* prompt;

    Ck_ValidationRes (*validator)(char*);

    void (*on_before_prompt)();
    void (*on_after_prompt)();
} Ck_InputParams;

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

void ck_input(char* buffer, size_t buffer_size, Ck_InputParams params) {
    if (NULL != params.prompt) {
        printf("%s", params.prompt);
    }

    size_t cursor = 0;
    char c = 0;

    while (c != '\n') {
        if (c == '\b') {
            if (cursor > 0) {
                buffer[cursor - 1] = 0;
                cursor--;
            }

            c = _ck_getch();
            continue;
        }
        if (cursor == buffer_size - 2) {
            c = _ck_getch();
            continue;
        }

        buffer[cursor] = c;

        Ck_ValidationRes res = params.validator(buffer);

        if (res.status == CK_INPUT_ERROR) {
            buffer[cursor] = 0;
        } else {
            printf("%c", c);
            cursor++;
        }

        c = _ck_getch();
    }

    printf("\n");
}

#endif /* __CKASTAL_INPUT_C__ */
