#ifndef __CKASTAL_LIST_SINGLE_CORE_C__
#define __CKASTAL_LIST_SINGLE_CORE_C__

#include "../../core/data.c"
#include "../../core/init.c"

typedef enum Ck_ListMode {
    CK_LIST_MODE_LINEAR = 0,
    CK_LIST_MODE_CIRCULAR,
} Ck_ListMode;

typedef struct Ck_ListNode {
    Ck_Data data;
    struct Ck_ListNode* next;
} Ck_ListNode;

typedef struct Ck_List {
    size_t length;
    Ck_ListMode mode;
    Ck_ListNode* head;
    Ck_ListNode* tail;
    Ck_DataType* type;
} Ck_List;

/**
 * Utility functions.
 */

/**
 * @brief Creates a new node.
 *
 * @param data The pointer to store as the data address.
 * @return Ck_ListNode*
 */
Ck_ListNode* _ck_list_node_new(void* data) {
    Ck_ListNode* node = (Ck_ListNode*)ck_memory.alloc(sizeof(Ck_ListNode));
    node->data = data;
    node->next = (Ck_ListNode*)NULL;
    return node;
}

void _ck_list_push_head(Ck_List* list, Ck_ListNode* node) {
    list->head = node;
    list->tail = list->head;

    if (list->mode == CK_LIST_MODE_CIRCULAR) {
        list->head->next = list->tail;
        list->tail->next = list->head;
    }

    list->length += 1;
}

/**
 * @brief Initializes a new list.
 *
 * @param type Data type configuration for the list elements.
 * @param mode List mode (linear or circular).
 * @return Ck_List Initialized list structure.
 */
Ck_List ck_list_init(Ck_DataType* type, Ck_ListMode mode) {
    Ck_List list = {
        .type = type,
        .head = (Ck_ListNode*)NULL,
        .tail = (Ck_ListNode*)NULL,
        .length = 0,
        .mode = mode,
    };
    return list;
}

/**
 * @brief Appends an element to the end of the list.
 *
 * @param list Target list.
 * @param data Data to append.
 */
void ck_list_push(Ck_List* list, void* data) {
    Ck_ListNode* node = _ck_list_node_new(data);

    if (!list->head) {
        _ck_list_push_head(list, node);
        return;
    }

    list->tail->next = node;
    list->tail = node;

    if (list->mode == CK_LIST_MODE_CIRCULAR) {
        list->tail->next = list->head;
    }

    list->length += 1;
}

/**
 * @brief Prepends an element to the beginning of the list.
 *
 * @param list Target list.
 * @param data Data to prepend.
 */
void ck_list_unshift(Ck_List* list, void* data) {
    Ck_ListNode* node = _ck_list_node_new(data);

    if (!list->head) {
        _ck_list_push_head(list, node);
        return;
    }

    node->next = list->head;
    list->head = node;

    if (list->mode == CK_LIST_MODE_CIRCULAR) {
        list->tail->next = list->head;
    }

    list->length += 1;
}

/**
 * @brief Removes and returns the last element from the list.
 *
 * @param list Target list.
 * @return void* Data from the removed node, or NULL if list is empty.
 */
void* ck_list_pop(Ck_List* list) {
    if (!list->head) {
        return NULL;
    }

    void* data = list->tail->data;

    /* Single element case. */
    if (list->head == list->tail) {
        ck_memory.free(list->tail);
        list->head = NULL;
        list->tail = NULL;
        list->length = 0;
        return data;
    }

    /* Find second-to-last node. */
    Ck_ListNode* current = list->head;
    while (current->next != list->tail) {
        current = current->next;
    }

    ck_memory.free(list->tail);
    list->tail = current;
    list->tail->next = (list->mode == CK_LIST_MODE_CIRCULAR) ? list->head : NULL;
    list->length -= 1;

    return data;
}

/**
 * @brief Removes and returns the first element from the list.
 *
 * @param list Target list.
 * @return void* Data from the removed node, or NULL if list is empty.
 */
void* ck_list_shift(Ck_List* list) {
    if (!list->head) {
        return NULL;
    }

    void* data = list->head->data;
    Ck_ListNode* old_head = list->head;

    /* Single element case. */
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
        list->length = 0;
        ck_memory.free(old_head);
        return data;
    }

    list->head = list->head->next;

    if (list->mode == CK_LIST_MODE_CIRCULAR) {
        list->tail->next = list->head;
    }

    list->length -= 1;
    ck_memory.free(old_head);

    return data;
}

/**
 * @brief Gets the element at the specified index.
 *
 * @param list Target list.
 * @param index Zero-based index.
 * @return void* Data at the index, or NULL if index is out of bounds.
 */
void* ck_list_get(Ck_List* list, size_t index) {
    if (index >= list->length) {
        return NULL;
    }

    Ck_ListNode* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

/**
 * @brief Inserts an element at the specified index.
 *
 * @param list Target list.
 * @param index Zero-based index where to insert.
 * @param data Data to insert.
 * @return int 0 on success, -1 on failure.
 */
int ck_list_insert(Ck_List* list, size_t index, void* data) {
    if (index > list->length) {
        return -1;
    }

    if (index == 0) {
        ck_list_unshift(list, data);
        return 0;
    }

    if (index == list->length) {
        ck_list_push(list, data);
        return 0;
    }

    Ck_ListNode* node = _ck_list_node_new(data);
    Ck_ListNode* current = list->head;

    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }

    node->next = current->next;
    current->next = node;
    list->length += 1;

    return 0;
}

/**
 * @brief Removes the element at the specified index.
 *
 * @param list Target list.
 * @param index Zero-based index of element to remove.
 * @return void* Data from the removed node, or NULL if index is out of bounds.
 */
void* ck_list_remove(Ck_List* list, size_t index) {
    if (index >= list->length) {
        return NULL;
    }

    if (index == 0) {
        return ck_list_shift(list);
    }

    if (index == list->length - 1) {
        return ck_list_pop(list);
    }

    Ck_ListNode* current = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }

    Ck_ListNode* to_remove = current->next;
    void* data = to_remove->data;
    current->next = to_remove->next;
    ck_memory.free(to_remove);
    list->length -= 1;

    return data;
}

/**
 * @brief Reverses the list in place.
 *
 * @param list Target list.
 */
void ck_list_reverse(Ck_List* list) {
    if (!list->head || list->length == 1) {
        return;
    }

    Ck_ListNode* prev = NULL;
    Ck_ListNode* current = list->head;
    Ck_ListNode* next = NULL;

    list->tail = list->head;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;

        /* Break circular reference during reversal. */
        if (list->mode == CK_LIST_MODE_CIRCULAR && current == list->head) {
            break;
        }
    }

    list->head = prev;

    if (list->mode == CK_LIST_MODE_CIRCULAR) {
        list->tail->next = list->head;
    }
}

/**
 * @brief Clears all elements from the list.
 *
 * @param list Target list.
 */
void ck_list_clear(Ck_List* list) {
    Ck_ListNode* node = list->head;
    Ck_ListNode* sentinel = (list->mode == CK_LIST_MODE_CIRCULAR) ? list->head : NULL;
    int first = 1;

    while (node != NULL) {
        /* Break on second encounter of head in circular mode. */
        if (list->mode == CK_LIST_MODE_CIRCULAR && node == sentinel && !first) {
            break;
        }
        first = 0;

        Ck_ListNode* next = node->next;

        if (list->type && list->type->free) {
            list->type->free(node->data);
        }

        ck_memory.free(node);
        node = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

/**
 * @brief Creates a shallow copy of the list.
 *
 * @param list Source list.
 * @return Ck_List New list with copied structure (data pointers are shared).
 */
Ck_List ck_list_copy(Ck_List* list) {
    Ck_List copy = ck_list_init(list->type, list->mode);
    Ck_ListNode* node = list->head;
    Ck_ListNode* sentinel = (list->mode == CK_LIST_MODE_CIRCULAR) ? list->head : NULL;
    int first = 1;

    while (node != NULL) {
        /* Break on second encounter of head in circular mode. */
        if (list->mode == CK_LIST_MODE_CIRCULAR && node == sentinel && !first) {
            break;
        }
        first = 0;

        ck_list_push(&copy, node->data);
        node = node->next;
    }

    return copy;
}

/**
 * @brief Frees the list and all its nodes.
 *
 * @param list_ptr Pointer to list pointer (will be set to NULL).
 */
void ck_list_free(Ck_List** list_ptr) {
    if (!list_ptr || !(*list_ptr)) {
        return;
    }

    Ck_List* list = *list_ptr;
    ck_list_clear(list);
    *list_ptr = NULL;
}

#endif /* __CKASTAL_LIST_SINGLE_CORE_C__ */
