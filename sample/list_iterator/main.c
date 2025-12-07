#include <stdio.h>
#include <stdlib.h>

#include "../../include/linked_list.c"
#include "../../include/syntax_utils.h"

Ck_DataType integer = {
    .size = sizeof(int),
    .to_str = NULL,
};

int main(void) {
    Ck_List list = ck_list_init(&integer, CK_LIST_MODE_LINEAR);
    Ck_Iterator iterator = ck_list_iterator(&list);

    int integers[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0; i < 10; i++) {
        ck_list_push(&list, &integers[i]);
    }

    foreach (Ck_ListNode *, node, iterator) {
        printf("integers[%zu] = %d\n", iterator._index, deref(node->data, int));
    }

    return 0;
}