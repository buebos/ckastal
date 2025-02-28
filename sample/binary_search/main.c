#include <stdio.h>
#include <string.h>

#include "../../include/array.c"
#include "../../include/init.c"
#include "../../include/input.c"
#include "../../include/terminal_utils.c"

typedef struct User {
    char id[9];
    char name[256];
} User;

CK_ARRAY_DECLARE_BINARY_SEARCH_FN(
    User* user_binary_search_by_id(User* array, size_t array_len, char* id),
    { order = strcmp(id, array[index].id); }

);

Ck_ValidationRes is_valid_id(char* input) {
    Ck_ValidationRes res = {
        .status = CK_VALIDATION_ERROR,
        .message = NULL,
    };

    size_t i = 0;

    while (input[i]) {
        if (i > 7) {
            return res;
        }
        if (input[i] < '0' || input[i] > '9') {
            return res;
        }

        i++;
    }

    res.status = CK_VALIDATION_OK;

    return res;
}

Ck_ValidationRes is_valid_menu(char* input) {
    Ck_ValidationRes res = {
        .status = CK_VALIDATION_ERROR,
        .message = NULL,
    };

    if (
        strcmp(input, "1") == 0 ||
        strcmp(input, "2") == 0 ||
        strcmp(input, "3") == 0

    ) {
        res.status = CK_VALIDATION_OK;
    }

    return res;
}

int main(void) {
    User users[20] = {
        {.name = "Gabriel Torres"},
        {.name = "Alice Johnson"},
        {.name = "Bob Smith"},
        {.name = "Charlie Brown"},
        {.name = "David White"},
        {.name = "Emma Davis"},
        {.name = "Frank Wilson"},
        {.name = "Grace Hall"},
        {.name = "Henry Adams"},
        {.name = "Ivy Thompson"},
        {.name = "Jack Martinez"},
        {.name = "Karen Lewis"},
        {.name = "Liam Walker"},
        {.name = "Mia Young"},
        {.name = "Noah Scott"},
        {.name = "Olivia King"},
        {.name = "Paul Green"},
        {.name = "Quinn Baker"},
        {.name = "Ryan Nelson"},
        {.name = "Sophia Carter"},
    };

    const int start_id = 10000000;

    for (int i = 0; i < 20; i++) {
        snprintf(users[i].id, sizeof(users[i].id), "%08d", start_id + i);
    }

    bool app_persist = true;

    while (app_persist) {
        char buffer[256] = {0};

        clear();
        printf("Welcome to user registry!\n\n");

        printf("1) Search for registry by ID\n");
        printf("2) See all registries\n");
        printf("3) Quit\n\n");

        ck_input(
            buffer,
            256,
            (Ck_InputParams){
                .prompt = "Type an option: ",
                .validator = is_valid_menu,
            }

        );

        short int menu = atoi(buffer);

        switch (menu) {
            case 1:
                clear();

                size_t i = 0;
                while (buffer[i] != 0) {
                    buffer[i] = 0;
                    i++;
                }

                ck_input(
                    buffer,
                    256,
                    (Ck_InputParams){
                        .validator = is_valid_id,
                        .prompt = "Type a valid id (only numbers): ",
                    }

                );

                User* user = user_binary_search_by_id(users, 20, buffer);

                if (user) {
                    printf("User {id: %s, name: %s}\n", user->id, user->name);
                } else {
                    printf("Not found.\n");
                }

                ck_input_to_continue("Type enter to return to main menu: ");

                break;
            case 2:
                clear();

                for (size_t i = 0; i < 20; i++) {
                    printf("User {id: %s, name: %s}\n", users[i].id, users[i].name);
                }

                printf("\n");

                ck_input_to_continue("Type enter to return to main menu: ");

                break;
            case 3:
                printf("See you in space cowboy!\n");

                return 0;
            default:
                break;
        }
    }

    return 0;
}
