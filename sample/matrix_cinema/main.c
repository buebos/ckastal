#include <stdio.h>

#include "../../include/input.c"
#include "../../include/terminal_utils.c"
#include "components/Theatre.c"
#include "core/app.c"
#include "view/CancelPurchase.c"
#include "view/ChooseSeats.c"
#include "view/DisplaySeats.c"
#include "view/Checkout.c"
#include "view/Home.c"

int main(void) {
    App app = {
        .seats = (TheatreSeat*)(TheatreSeat[ROWS_COUNT][COLUMNS_COUNT]){0},
        .menu = MENU_HOME,
        .persist = true,
        .cart = (Purchase){0},
    };

    while (app.persist) {
        Home(&app);

        switch (app.menu) {
            case MENU_CHOOSE_TICKETS:
                ChooseSeats(&app);
                break;
            case MENU_CANCEL_PURCHASE:
                CancelPurchase(&app);
                break;
            case MENU_DISPLAY_SEATS:
                DisplaySeats(&app);
                break;
            case MENU_CHECKOUT:
                Checkout(&app);
                break;
            default:
                printf("See you in space cowboy!\n");
                return 0;
        }
    }
}
