#ifndef __COMPONENTS_SEATS_C__
#define __COMPONENTS_SEATS_C__

#include "../../../include/init.c"
#include "../core/app.c"
#include "../model/TheatreSeat.c"

void Theatre(TheatreSeat *seats) {
    unsigned short width = 6 * COLUMNS_COUNT + 5;
    unsigned long screen_string_length = sizeof("SCREEN");

    printf("|   ");
    for (size_t j = 0; j < COLUMNS_COUNT - 4; j++) {
        printf(" SCREEN ");
    }
    printf("  |\n");

    printf("|_____");
    for (size_t j = 0; j < COLUMNS_COUNT - 1; j++) {
        printf("_____");
    }
    printf("___|");

    printf("\n\n");

    for (size_t i = 0; i < ROWS_COUNT; i++) {
        printf("| %c | ", 'A' + ((int)i));

        for (size_t j = 0; j < COLUMNS_COUNT; j++) {
            printf("%s\033[0m | ",
                   seats[COLUMNS_COUNT * i + j].is_bussy ? "\033[31mXX" : "\033[32m--");
        }

        printf("\n");
    }

    printf("|----");
    for (size_t j = 0; j < COLUMNS_COUNT - 1; j++) {
        printf("-----");
    }
    printf("----|");

    printf("\n");

    printf("|   | ");

    for (size_t j = 0; j < COLUMNS_COUNT; j++) {
        printf("%.2zu | ", j + 1);
    }

    printf("\n\n");

    printf("XX: Bussy | --: Available\n\n");
}

#endif /* __COMPONENTS_SEATS_C__ */
