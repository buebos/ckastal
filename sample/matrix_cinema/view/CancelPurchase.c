#ifndef __VIEW_CANCEL_PURCHASE_C__
#define __VIEW_CANCEL_PURCHASE_C__

#include "../../../include/input.c"
#include "../../../include/terminal_utils.c"
#include "../components/Cart.c"
#include "../core/app.c"

void CancelPurchase(App *app) {
    bool is_canceling = true;

    while (is_canceling) {
        if (!app->cart.seats_len) {
            clear();

            Cart(&app->cart);

            printf("\n");

            printf("There is no cart nor purchase to cancel\n");

            is_canceling =
                !ck_input_yes_or_no("Do you want to go back? (y/n) ");

            continue;
        }

        clear();
        Cart(&app->cart);
        printf("\n");

        is_canceling =
            ck_input_yes_or_no("Are you sure you want to cancel the current purchase? (y/n) ");

        if (is_canceling) {
            purchase_clear(&app->cart);

            for (size_t i = 0; i < ROWS_COUNT; i++) {
                for (size_t j = 0; j < COLUMNS_COUNT; j++) {
                    app->seats[COLUMNS_COUNT * i + j].is_bussy = false;
                }
            }

            break;
        }
    }
}

#endif /* __VIEW_CANCEL_PURCHASE_C__ */
