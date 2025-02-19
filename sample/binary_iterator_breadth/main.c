#include <stdio.h>
#include <stdlib.h>

#include "../../include/binary_tree.c"
#include "../../include/syntax_utils.h"

Ck_DataTypeCompareResult compare(void *a, void *b) {
    int l = deref(a, int);
    int r = deref(b, int);

    if (l < r) {
        return CK_LT;
    }
    if (l > r) {
        return CK_GT;
    }
    return CK_EQ;
}

Ck_DataType integer = {
    .size = sizeof(int),
    .to_str = NULL,
    .compare = compare,
};

int main(void) {
    Ck_Treebi tree = ck_treebi_init(&integer);
    Ck_Queue queue = ck_queue_init(&integer);
    Ck_Iterator iterator = ck_treebi_iterator_breadth(&tree, &queue);

    int integers[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (int i = 0; i < 10; i++) {
        ck_treebi_insert(&tree, &integers[i]);
    }

    foreach (Ck_TreebiNode *, node, iterator) {
        printf("Tree Node %zu: %d\n", iterator._index, deref(node->data, int));
    }

    return 0;
}
