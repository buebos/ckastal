#ifndef __VIEW_DISPLAY_SEATS_C__
#define __VIEW_DISPLAY_SEATS_C__

#include "../../../include/input.c"
#include "../../../include/terminal_utils.c"
#include "../components/Theatre.c"
#include "../core/app.c"

void DisplaySeats(App *app) {
    bool is_viewing = true;

    while (is_viewing) {
        clear();
        Theatre(app->seats);

        is_viewing = !ck_input_yes_or_no("Type 'y' to go back: ");
    }
}

#endif /* __VIEW_DISPLAY_SEATS_C__ */
