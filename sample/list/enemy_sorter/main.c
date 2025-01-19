#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/linked_list.c"
#include "../../../include/syntax_utils.h"
#include "../../../include/terminal_utils.c"
#include "model/Enemy.c"
#include "view/EnemyList.c"

Ck_DataType enemy_data_type = {
    .size = sizeof(Enemy),
    .to_str = NULL,
    .compare = NULL,
};

int main(int argc, char** argv) {
    Ck_List enemies = ck_list_init(&enemy_data_type, CK_LIST_LINEAR);
    bool should_continue = true;

    for (int i = 1; i <= 20; i++) {
        /**
         * Calls based on i to get a kind of scattered sample. Could use
         * rand function but I think it would be an overkill for this
         * purpose lol
         */
        Position position = {
            .x = (100 - i) % i + 5,
            .y = i,
            .z = i,
        };

        ck_list_push(&enemies, enemy_new(i, (100 - i) % i + 5, (EnemyType)(i % 3), position));
    }

    while (should_continue) {
        Ck_List sorted_enemies = ck_list_copy(&enemies);
        int sort_strategy = 0;
        char continue_ans;

        clear();
        printf("[INFO]: Original list:\n");
        EnemyList(&enemies);
        printf("\n");
        printf("[INFO]: Select a sort strategy:\n[1]: Asc by health\n[2]: Desc by health\n[3]: Asc by tag (Creeper, Skeleton, Zombie)\n[4]: Desc by tag (Zombie, Skeleton, Creeper)\n\n[INPUT]: ");

        scanf("%d", &sort_strategy);
        clear();

        printf("[INFO]: Sorted");

        ck_list_bubblesort(sorted_enemies, Enemy*, {
            switch (sort_strategy) {
                case 1:
                    printf(" asc by health:\n");
                    order = a->health - b->health;
                    break;
                case 2:
                    printf(" desc by health:\n");
                    order = b->health - a->health;
                    break;
                case 3:
                    printf(" asc by tag:\n");
                    order = enemy_type_tags[a->type][0] - enemy_type_tags[b->type][0];
                    break;
                default:
                    printf(" desc by tag:\n");
                    order = enemy_type_tags[b->type][0] - enemy_type_tags[a->type][0];
                    break;
            }
        });

        EnemyList(&sorted_enemies);
        printf("\n");

        // ck_list_free(&sorted_enemies);
        printf("[INFO]: Freed sorted enemy list\n");

        printf("[INPUT]: Do you want to continue? (y / n): ");
        scanf(" %c", &continue_ans);
        should_continue = continue_ans == "y"[0];
    }

    clear();
    // ck_list_free(&enemies);
    printf("[INFO]: Freed original enemy list\n");
    printf("[INFO]: Bye!\n");

    return 0;
}
