#ifndef __VIEW_CHOOSE_TICKETS_C__
#define __VIEW_CHOOSE_TICKETS_C__

#include "../../../include/input.c"
#include "../components/Cart.c"
#include "../core/app.c"

#define CHOOSE_SEATS_BUFFER_SIZE 3

Ck_ValidationRes ChooseSeats_is_valid_input_row(char *input) {
    return (Ck_ValidationRes){
        .status = input[1] == 0 && is_valid_row(input[0])
                      ? CK_VALIDATION_OK
                      : CK_VALIDATION_ERROR,
        0,
    };
}
Ck_ValidationRes ChooseSeats_is_valid_input_column(char *input) {
    return (Ck_ValidationRes){
        .status = ((input[1] == '0' && input[2] == 0) || (input[1] == 0)) && is_valid_column(input[0])
                      ? CK_VALIDATION_OK
                      : CK_VALIDATION_ERROR,
        0,
    };
}

void ChooseSeatsDetails(App *app) {
    clear();
    Theatre(app->seats);
    Cart(&app->cart);
    printf("\n");
}

void ChooseSeats(App *app) {
    bool is_choosing_seat = true;

    while (is_choosing_seat) {
        char buffer[CHOOSE_SEATS_BUFFER_SIZE] = {0};

        ChooseSeatsDetails(app);

        char row = 0;
        size_t col = 0;

        ck_input(
            buffer,
            CHOOSE_SEATS_BUFFER_SIZE,
            (Ck_InputParams){
                .prompt = "Choose a row: ",
                .validator = ChooseSeats_is_valid_input_row,
                0,
            }

        );

        /** Convert to lower case */
        if (buffer[0] >= 'A' && buffer[0] <= 'Z') {
            buffer[0] += 32;
        }

        row = buffer[0];

        ck_input(
            buffer,
            CHOOSE_SEATS_BUFFER_SIZE,
            (Ck_InputParams){
                .prompt = "Choose a column: ",
                .validator = ChooseSeats_is_valid_input_column,
                0,
            }

        );

        col = (size_t)atoi(buffer);

        if (!is_seat_bussy(app, row, col)) {
            purchase_push_seat(
                &app->cart,
                (PurchaseSeat){
                    .price = get_seat_price(row),
                    .seat_row = row,
                    .seat_column = col,
                }

            );

            toggle_seat_bussy(app, row, col);

            ChooseSeatsDetails(app);
        } else {
            printf("That seat is already taken\n");
        }

        is_choosing_seat = ck_input_yes_or_no("Do you want to choose another seat? (y/n) ");
    }
}

#endif /* __VIEW_CHOOSE_TICKETS_C__ */
