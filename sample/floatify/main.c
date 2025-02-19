#include <regex.h>
#include <stdio.h>

#include "../../include/input.c"

int main(void) {
    char buffer[128] = {0};

    ck_input(
        buffer,
        128,
        (Ck_InputParams){
            .prompt = "Type a valid float: ",
            .validator = ck_validation_float,
            0,
        }

    );

    printf("%s\n", buffer);

    return 0;
}