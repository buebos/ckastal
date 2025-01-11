#ifndef __CKASTAL_TREEBI_FUNCTION_ITERATOR_BREADTH_C__
#define __CKASTAL_TREEBI_FUNCTION_ITERATOR_BREADTH_C__

#include "../../../../core/iterator.c"
#include "../../../queue/simple/index.c"
#include "../core/main.c"

void *_ck_treebi_iterator_breadth_next(Ck_Iterator *self) {
    Ck_Queue *queue = self->_ctx;
    Ck_Treebi *tree = self->_collection;

    if (self->_index == tree->_size - 1) {
        self->_cursor = NULL;
        return NULL;
    }

    Ck_TreebiNode *node =
        ck_queue_size(queue) == 0
            ? ((Ck_Treebi *)self->_collection)->_root
            : ck_queue_dequeue(queue);

    self->_cursor = node;
    self->_index += 1;

    if (node->left) {
        ck_queue_enqueue(queue, node->left);
    }
    if (node->right) {
        ck_queue_enqueue(queue, node->right);
    }

    return node;
}

Ck_Iterator ck_treebi_iterator_breadth(Ck_Treebi *tree, Ck_Queue *queue) {
    Ck_Iterator iterator = {
        ._collection = tree,

        ._cursor = NULL,
        ._index = 0,

        ._ctx = queue,

        .next = _ck_treebi_iterator_breadth_next,
    };

    return iterator;
}

#endif /** __CKASTAL_TREEBI_FUNCTION_ITERATOR_BREADTH_C__ */
