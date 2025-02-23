#include <stdio.h>
#include <stdlib.h>

#include "../../include/input.c"

int main(void) {
    float value = ck_input_float("Type a valid float: ");

    printf("The result of parsing that float is: %.20lf\n", value);

    return 0;
}