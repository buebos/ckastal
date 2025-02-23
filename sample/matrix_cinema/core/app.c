#ifndef __CORE_CONSTANT_C__
#define __CORE_CONSTANT_C__

#include "../model/Purchase.c"
#include "../model/TheatreSeat.c"

#define ROWS_COUNT 9
#define COLUMNS_COUNT 10

typedef enum Menu {
    MENU_HOME = 0,
    MENU_CHOOSE_TICKETS,
    MENU_CANCEL_PURCHASE,
    MENU_DISPLAY_SEATS,
    MENU_CHECKOUT,
} Menu;

typedef struct App {
    TheatreSeat* seats;
    Menu menu;
    bool persist;

    Purchase cart;
} App;

bool is_valid_row(char row) {
    if (row >= 'A' && row <= 'Z') {
        row += 32;
    }

    return row >= 'a' && row < ('a' + ROWS_COUNT);
}
bool is_valid_column(char column) {
    return (column >= '1' && column < ('1' + COLUMNS_COUNT));
}

size_t get_seat_price(char row) {
    if (row == 'a') {
        return 1000;
    }
    if (row == 'b') {
        return 750;
    }
    if (row == 'c') {
        return 500;
    }

    return 300;
}

bool is_seat_bussy(App* app, char row, size_t col) {
    size_t i = row - 'a';
    size_t j = col - 1;

    return app->seats[COLUMNS_COUNT * i + j].is_bussy;
}

void toggle_seat_bussy(App* app, char row, size_t col) {
    size_t i = row - 'a';
    size_t j = col - 1;

    app->seats[COLUMNS_COUNT * i + j].is_bussy = !app->seats[COLUMNS_COUNT * i + j].is_bussy;
}

#endif /* __CORE_CONSTANT_C__ */
