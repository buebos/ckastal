#ifndef __CKASTAL_LIST_DOUBLE_C__
#define __CKASTAL_LIST_DOUBLE_C__

#include <stdio.h>

#include "../../core/data.c"
#include "../../core/init.c"

typedef enum Ck_ListDoubleMode {
    CK_LIST_DOUBLE_MODE_LINEAR = 0,
    CK_LIST_DOUBLE_MODE_CIRCULAR,
} Ck_ListDoubleMode;

typedef struct CK_ListDoubleNode {
    Ck_Data data;
    struct CK_ListDoubleNode* next;
    struct CK_ListDoubleNode* prev;
} CK_ListDoubleNode;

typedef struct Ck_ListDouble {
    size_t length;

    Ck_ListDoubleMode mode;

    CK_ListDoubleNode* head;
    CK_ListDoubleNode* tail;

    struct _Ck_ListDouble {
        Ck_DataType type;
    } _;
} Ck_ListDouble;

typedef struct Ck_ListDoubleInitParams {
    Ck_ListDoubleMode mode;
    Ck_DataType type;
} Ck_ListDoubleInitParams;

Ck_ListDouble ck_listdouble_init(Ck_ListDoubleInitParams params) {
    Ck_ListDouble list = {
        .length = 0,
        .head = NULL,
        .tail = NULL,
        .mode = params.mode,
        ._ = {
            .type = params.type,
        },
    };

    return list;
}

Ck_Data ck_listdouble_get(Ck_ListDouble* list, size_t i) {
    if (i >= list->length) {
        return NULL;
    }

    CK_ListDoubleNode* current = list->head;

    for (size_t j = 0; j < i; j++) {
        current = current->next;
    }

    return current->data;
}

void ck_listdouble_push(Ck_ListDouble* list, Ck_Data data) {
    CK_ListDoubleNode* node = ck_memory.alloc(sizeof(CK_ListDoubleNode));
    *node = (CK_ListDoubleNode){.data = data, .prev = NULL, .next = NULL};

    if (NULL == list->head) {
        list->head = node;
        list->tail = node;

        if (list->mode == CK_LIST_DOUBLE_MODE_CIRCULAR) {
            list->tail->next = list->head;
            list->head->prev = list->tail;
        }

        list->length++;
        return;
    }

    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;

    if (list->mode == CK_LIST_DOUBLE_MODE_CIRCULAR) {
        list->tail->next = list->head;
        list->head->prev = list->tail;
    }

    list->length++;
    return;
}

#endif /** __CKASTAL_LIST_DOUBLE_C__ */
