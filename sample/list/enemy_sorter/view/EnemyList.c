#ifndef __ENEMY_LIST_C__
#define __ENEMY_LIST_C__

#include "../../../../include/linked_list.c"
#include "../../../../include/syntax_utils.h"
#include "../model/Enemy.c"

void EnemyList(Ck_List* list) {
    printf("[");
    if (list->length > 0) {
        printf("\n");
    }

    Ck_Iterator iterator = ck_list_iterator(list);

    foreach (Ck_ListNode*, node, iterator) {
        Enemy* enemy = cast(node->data, Enemy*);

        printf("\t{ id: %d,\t tag: %s,\t health: %d, coords: (%d, %d, %d) }", enemy->id, enemy_type_tags[enemy->type], enemy->health, enemy->position.x, enemy->position.y, enemy->position.z);

        if (iterator._index < list->length - 1) {
            printf(" -> \n");
        }
    }

    if (list->length > 0) {
        printf("\n");
    }

    printf("]\n");
}

#endif