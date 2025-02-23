#ifndef __VIEW_HOME_C__
#define __VIEW_HOME_C__

#include <stdlib.h>

#include "../../../include/input.c"
#include "../core/app.c"

#define BUFFER_SIZE 256
#define OPTION_EXIT_APPLICATION 5

Ck_ValidationRes is_valid_option(char *buffer) {
    int input = atoi(buffer);

    Ck_ValidationRes res = {
        .status = CK_VALIDATION_OK,
        .message = NULL,
    };

    if (input > OPTION_EXIT_APPLICATION || input < MENU_CHOOSE_TICKETS) {
        res.status = CK_VALIDATION_ERROR;
    }

    return res;
}

void Home(App *app) {
    clear();

    char buffer[BUFFER_SIZE] = {0};

    printf("Welcome to matrix cinema!\n\n");

    printf("Current menu: HOME\n\n");
    printf("1) Choose tickets\n");
    printf("2) Cancel purchase\n");
    printf("3) Display seats\n");
    printf("4) Checkout\n");
    printf("5) Exit application\n\n");

    ck_input(
        buffer,
        BUFFER_SIZE,
        (Ck_InputParams){
            .prompt = "Select an option (1 to 5): ",
            .validator = is_valid_option,
            0,
        }

    );

    int option = atoi(buffer);

    app->menu = option;

    if (option == OPTION_EXIT_APPLICATION) {
        app->persist = false;
    }
}

#endif /* __VIEW_HOME_C__ */
