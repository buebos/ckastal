#ifndef __COMPONENTS_CART_C__
#define __COMPONENTS_CART_C__

#include <stdio.h>
#include <string.h>

#include "../core/app.c"

void Cart(Purchase *purchase) {
    printf("-----------------------\n");
    printf("Cart\n\n");

    if (!purchase->seats) {
        printf("(Cart is empty)\n");
        printf("-----------------------\n");

        return;
    }

    size_t count = 0;
    PurchaseSeat *seat = purchase->seats;

    printf("%.10s %.10s %.10s\n", "Row", "Column", "Price");

    while (count < purchase->seats_len) {
        printf("%-3c %-6zu %-10.2lf\n", seat->seat_row - 32, seat->seat_column, seat->price);
        seat++;
        count++;
    }

    printf("\nTotal seats: %zu\n", count);
    printf("Total: $%.2lf\n", purchase_get_total(purchase, purchase->seats_len));

    printf("-----------------------\n");
}

#endif /* __COMPONENTS_CART_C__ */
