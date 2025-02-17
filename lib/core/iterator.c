#ifndef __CKASTAL_CORE_ITERATOR_C__
#define __CKASTAL_CORE_ITERATOR_C__

#include "init.c"

#define _ck_foreach(type, node, iterator) \
    for (type node = iterator.next(&iterator); node != NULL; node = iterator.next(&iterator))

typedef struct Ck_Iterator Ck_Iterator;

typedef void *(*Ck_IteratorNext)(Ck_Iterator *self);

typedef struct Ck_Iterator {
    void *_collection;

    void *_cursor;
    size_t _index;

    void *_ctx;

    Ck_IteratorNext next;
} Ck_Iterator;

#endif /** __CKASTAL_CORE_ITERATOR_C__ */