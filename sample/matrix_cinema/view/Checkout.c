#ifndef __VIEW_CHECKOUT_C__
#define __VIEW_CHECKOUT_C__

#include "../../../include/input.c"
#include "../../../include/terminal_utils.c"
#include "../components/Cart.c"
#include "../core/app.c"

void Checkout(App *app) {
    bool is_buying = false;

    clear();
    Cart(&app->cart);
    printf("\n");

    if (!app->cart.seats_len) {
        is_buying = ck_input_yes_or_no("Cart is empty. Cannot checkout. Type 'y' or 'n' to return to home: ");
        return;
    }

    is_buying = ck_input_yes_or_no("Do you want to confirm this purchase? This will confirm the purchase and end the application (y/n) ");

    if (is_buying) {
        printf("Thanks for your purchase! See you in space cowboy\n");

        app->persist = false;
    }
}

#endif /* __VIEW_CHECKOUT_C__ */
