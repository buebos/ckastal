#ifndef __CKASTAL_QUEUE_C__
#define __CKASTAL_QUEUE_C__

#include "../../list_single/index.c"

#include "../../../core/data.c"

typedef struct Ck_Queue {
    Ck_List list;
} Ck_Queue;

Ck_Queue ck_queue_init(Ck_DataType* type) {
    Ck_Queue queue = {
        .list = ck_list_init(type, CK_LIST_LINEAR),
    };
    return queue;
}

void ck_queue_enqueue(Ck_Queue* queue, void* data) {
    ck_list_push(&queue->list, data);
}

void* ck_queue_dequeue(Ck_Queue* queue) {
    if (queue->list.length == 0) {
        return NULL;
    }

    Ck_ListNode* node = queue->list.head;
    void* data = node->data;

    queue->list.head = node->next;

    if (queue->list.length == 1) {
        queue->list.tail = NULL;
    }

    queue->list.length -= 1;

    ck_memory.free(node);

    return data;
}

size_t ck_queue_size(Ck_Queue* queue) {
    return queue->list.length;
}

void ck_queue_free(Ck_Queue* queue) {
}

#endif /* __CKASTAL_QUEUE_C__ */
