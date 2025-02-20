#include <stdio.h>

#include "../../include/input.c"

#define BUFFER_SIZE 128
#define ID_SIZE 8

char get_first(char *buffer) {
    size_t length = 0;

    while (buffer[length] != 0) {
        length++;
    }

    return ((int)(length % (size_t)10)) + 48;
}
char get_second(char *buffer) {
    size_t length = 0;

    size_t first_letter_index = 0;
    bool has_first_letter_index = false;

    while (buffer[length] != 0) {
        char c = buffer[length];

        /** Convert to lower case asuming ASCII */
        if (c <= 'Z' && c >= 'A') {
            c += 32;
        }

        if (c >= 'a' && c <= 'z' && !has_first_letter_index) {
            first_letter_index = length;
            has_first_letter_index = true;
        }

        length++;
    }

    if (has_first_letter_index) {
        return buffer[first_letter_index] <= 'Z'
                   ? buffer[first_letter_index]
                   : (buffer[first_letter_index] - 32);
    }

    return buffer[0];
}
char get_third(char *buffer) {
    size_t length = 0;
    size_t vocals_count = 0;

    while (buffer[length] != 0) {
        char c = buffer[length];

        /** Convert to lower case asuming ASCII */
        if (c <= 'Z' && c >= 'A') {
            c += 32;
        }

        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vocals_count++;
        }

        length++;
    }

    return vocals_count + 48;
}
char get_fourth(char *buffer) {
    size_t length = 0;
    bool has_letter_e = false;

    while (buffer[length] != 0) {
        char c = buffer[length];

        /** Convert to lower case asuming ASCII */
        if (c <= 'Z' && c >= 'A') {
            c += 32;
        }

        if (c == 'e') {
            has_letter_e = true;
        }

        length++;
    }

    return has_letter_e ? '!' : '#';
}
void set_last_three(char *buffer, char *id) {
    for (size_t i = 1; i <= 3; i++) {
        id[ID_SIZE - 1 - i] = buffer[(id[0] - 48) - 4 + i];
    }

}

int main(void) {
    char buffer[BUFFER_SIZE] = {0};
    char id[ID_SIZE] = {0};

    ck_input(
        buffer,
        BUFFER_SIZE,
        (Ck_InputParams){
            .prompt = "[INPUT]: Type a social network name (char limit 128): ",
            0,
        }

    );

    id[0] = get_first(buffer);
    id[1] = get_second(buffer);
    id[2] = get_third(buffer);
    id[3] = get_fourth(buffer);
    set_last_three(buffer, id);

    printf("[INFO]: Social network name: '%s'; result: '%s'\n", buffer, id);

    return 0;
}
