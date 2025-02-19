#ifndef __CKASTAL_PROVIDER_LIST_SINGLE_FUNCTION_ITERATOR_C__
#define __CKASTAL_PROVIDER_LIST_SINGLE_FUNCTION_ITERATOR_C__
#include "../../../../core/iterator.c"

#include "../core/main.c"

void* _ck_list_iterator_next(Ck_Iterator* self) {
    Ck_List* list = self->_collection;

    if (NULL == self->_cursor) {
        self->_index = 0;
        return (self->_cursor = list->head);
    }
    if (self->_index == list->length - 1) {
        self->_cursor = NULL;
        return NULL;
    }

    self->_cursor = ((Ck_ListNode*)self->_cursor)->next;
    self->_index += 1;

    return self->_cursor;
}

Ck_Iterator ck_list_iterator(Ck_List* list) {
    Ck_Iterator iterator = {
        ._cursor = NULL,
        ._collection = list,
        ._index = 0,
        .next = _ck_list_iterator_next,
    };

    return iterator;
}

#endif /** __CKASTAL_PROVIDER_LIST_SINGLE_FUNCTION_ITERATOR_C__ */