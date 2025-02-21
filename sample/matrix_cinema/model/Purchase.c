#ifndef __MODEL_PURCHASE_C__
#define __MODEL_PURCHASE_C__

#include <stdlib.h>

#include "../../../include/init.c"

typedef struct PurchaseSeat {
    float price;

    char seat_row;
    size_t seat_column;
} PurchaseSeat;

typedef struct Purchase {
    PurchaseSeat* seats;
    size_t seats_len;
} Purchase;

float purchase_get_total(Purchase* purchase, size_t purchase_items_len) {
    float total = 0;
    PurchaseSeat* seat = purchase->seats;

    for (size_t i = 0; i < purchase_items_len; i++) {
        total += seat->price;
        seat++;
    }

    return total;
}

void purchase_push_seat(Purchase* purchase, PurchaseSeat seat) {
    purchase->seats = realloc(purchase->seats, sizeof(PurchaseSeat) * (purchase->seats_len + 1));
    purchase->seats[purchase->seats_len++] = seat;
}

void purchase_clear(Purchase* purchase) {
    free(purchase->seats);
    purchase->seats = NULL;
    purchase->seats_len = 0;
}

#endif /* __MODEL_PURCHASE_C__`ƒ */