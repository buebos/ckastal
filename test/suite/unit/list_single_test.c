#include <stdlib.h>

#include "../../../include/init.c"
#include "../../../include/linked_list.c"
#include "../../../vendor/ckester/include/index.c"

/* Initialization tests. */


TEST(list_init) {
    Ck_List list = ck_list_init(
        &(Ck_DataType){
            .compare = NULL,
            .size = sizeof(int),
            .to_str = NULL,
        },
        CK_LIST_MODE_LINEAR);

    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
    ASSERT_EQ(list.mode, CK_LIST_MODE_LINEAR);
    ASSERT_EQ(list.length, 0);
    ASSERT_EQ(list.type->size, sizeof(int));
    ASSERT_NULL(list.type->compare);
    ASSERT_NULL(list.type->to_str);
}

TEST(list_init_circular) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
    ASSERT_EQ(list.mode, CK_LIST_MODE_CIRCULAR);
    ASSERT_EQ(list.length, 0);
}

TEST(list_empty_initialization) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

/* Push/unshift tests. */

TEST(list_push_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)42);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ((long)list.head->data, 42);
    ASSERT_EQ((long)list.tail->data, 42);
    ASSERT_EQ(list.head, list.tail);
    ASSERT_NULL(list.tail->next);
}

TEST(list_unshift_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_unshift(&list, (void*)(long)42);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ((long)list.head->data, 42);
    ASSERT_EQ((long)list.tail->data, 42);
    ASSERT_EQ(list.head, list.tail);
}

TEST(list_push_multiple_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 1);
    ASSERT_EQ((long)list.tail->data, 3);
}

TEST(list_unshift_multiple_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_unshift(&list, (void*)(long)1);
    ck_list_unshift(&list, (void*)(long)2);
    ck_list_unshift(&list, (void*)(long)3);

    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 3);
    ASSERT_EQ((long)list.tail->data, 1);
}

TEST(list_push_and_unshift_100_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    for (int i = 1; i <= 100; i++) {
        ck_list_push(&list, (void*)(long)i);
        ck_list_unshift(&list, (void*)(long)i);

        ASSERT_EQ(list.length, i * 2);
        ASSERT_EQ((long)list.tail->data, i);
        ASSERT_EQ((long)list.head->data, i);
    }
}

TEST(list_alternating_push_unshift) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ASSERT_EQ(list.length, 1);
    ASSERT_EQ((long)list.tail->data, 1);

    ck_list_unshift(&list, (void*)(long)2);
    ASSERT_EQ(list.length, 2);
    ASSERT_EQ((long)list.head->data, 2);
    ASSERT_EQ((long)list.tail->data, 1);

    ck_list_push(&list, (void*)(long)3);
    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 2);
    ASSERT_EQ((long)list.tail->data, 3);
}

/* Circular mode tests. */

TEST(list_circular_push_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)42);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ(list.head, list.tail);
    ASSERT_EQ(list.head->next, list.head);
    ASSERT_EQ(list.tail->next, list.head);
}

TEST(list_circular_push_multiple_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 1);
    ASSERT_EQ((long)list.tail->data, 3);
    ASSERT_EQ(list.tail->next, list.head);
}

TEST(list_circular_unshift_maintains_circularity) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_unshift(&list, (void*)(long)0);

    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 0);
    ASSERT_EQ(list.tail->next, list.head);
}

/* Pop/shift tests. */

TEST(list_pop_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);
    ck_list_push(&list, (void*)(long)42);

    void* data = ck_list_pop(&list);

    ASSERT_EQ((long)data, 42);
    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

TEST(list_shift_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);
    ck_list_push(&list, (void*)(long)42);

    void* data = ck_list_shift(&list);

    ASSERT_EQ((long)data, 42);
    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

TEST(list_pop_multiple_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    void* data3 = ck_list_pop(&list);
    ASSERT_EQ((long)data3, 3);
    ASSERT_EQ(list.length, 2);

    void* data2 = ck_list_pop(&list);
    ASSERT_EQ((long)data2, 2);
    ASSERT_EQ(list.length, 1);

    void* data1 = ck_list_pop(&list);
    ASSERT_EQ((long)data1, 1);
    ASSERT_EQ(list.length, 0);
}

TEST(list_shift_multiple_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    void* data1 = ck_list_shift(&list);
    ASSERT_EQ((long)data1, 1);
    ASSERT_EQ(list.length, 2);

    void* data2 = ck_list_shift(&list);
    ASSERT_EQ((long)data2, 2);
    ASSERT_EQ(list.length, 1);

    void* data3 = ck_list_shift(&list);
    ASSERT_EQ((long)data3, 3);
    ASSERT_EQ(list.length, 0);
}

TEST(list_pop_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    void* data = ck_list_pop(&list);

    ASSERT_NULL(data);
    ASSERT_EQ(list.length, 0);
}

TEST(list_shift_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    void* data = ck_list_shift(&list);

    ASSERT_NULL(data);
    ASSERT_EQ(list.length, 0);
}

TEST(list_circular_pop_maintains_circularity) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ck_list_pop(&list);

    ASSERT_EQ(list.length, 2);
    ASSERT_EQ(list.tail->next, list.head);
}

TEST(list_circular_shift_maintains_circularity) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ck_list_shift(&list);

    ASSERT_EQ(list.length, 2);
    ASSERT_EQ(list.tail->next, list.head);
}

/* Get tests. */

TEST(list_get_valid_indices) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)10);
    ck_list_push(&list, (void*)(long)20);
    ck_list_push(&list, (void*)(long)30);

    ASSERT_EQ((long)ck_list_get(&list, 0), 10);
    ASSERT_EQ((long)ck_list_get(&list, 1), 20);
    ASSERT_EQ((long)ck_list_get(&list, 2), 30);
}

TEST(list_get_out_of_bounds) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)10);

    ASSERT_NULL(ck_list_get(&list, 1));
    ASSERT_NULL(ck_list_get(&list, 100));
}

TEST(list_get_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ASSERT_NULL(ck_list_get(&list, 0));
}

/* Insert tests. */

TEST(list_insert_at_beginning) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    int result = ck_list_insert(&list, 0, (void*)(long)1);

    ASSERT_EQ(result, 0);
    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 1);
}

TEST(list_insert_at_end) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);

    int result = ck_list_insert(&list, 2, (void*)(long)3);

    ASSERT_EQ(result, 0);
    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.tail->data, 3);
}

TEST(list_insert_in_middle) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)3);

    int result = ck_list_insert(&list, 1, (void*)(long)2);

    ASSERT_EQ(result, 0);
    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)ck_list_get(&list, 0), 1);
    ASSERT_EQ((long)ck_list_get(&list, 1), 2);
    ASSERT_EQ((long)ck_list_get(&list, 2), 3);
}

TEST(list_insert_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    int result = ck_list_insert(&list, 0, (void*)(long)42);

    ASSERT_EQ(result, 0);
    ASSERT_EQ(list.length, 1);
    ASSERT_EQ((long)list.head->data, 42);
}

TEST(list_insert_out_of_bounds) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);

    int result = ck_list_insert(&list, 5, (void*)(long)2);

    ASSERT_EQ(result, -1);
    ASSERT_EQ(list.length, 1);
}

/* Remove tests. */

TEST(list_remove_first_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    void* data = ck_list_remove(&list, 0);

    ASSERT_EQ((long)data, 1);
    ASSERT_EQ(list.length, 2);
    ASSERT_EQ((long)list.head->data, 2);
}

TEST(list_remove_last_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    void* data = ck_list_remove(&list, 2);

    ASSERT_EQ((long)data, 3);
    ASSERT_EQ(list.length, 2);
    ASSERT_EQ((long)list.tail->data, 2);
}

TEST(list_remove_middle_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    void* data = ck_list_remove(&list, 1);

    ASSERT_EQ((long)data, 2);
    ASSERT_EQ(list.length, 2);
    ASSERT_EQ((long)ck_list_get(&list, 0), 1);
    ASSERT_EQ((long)ck_list_get(&list, 1), 3);
}

TEST(list_remove_out_of_bounds) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);

    void* data = ck_list_remove(&list, 5);

    ASSERT_NULL(data);
    ASSERT_EQ(list.length, 1);
}

TEST(list_remove_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    void* data = ck_list_remove(&list, 0);

    ASSERT_NULL(data);
    ASSERT_EQ(list.length, 0);
}

/* Reverse tests. */

TEST(list_reverse_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_reverse(&list);

    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

TEST(list_reverse_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);
    ck_list_push(&list, (void*)(long)42);

    ck_list_reverse(&list);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ((long)list.head->data, 42);
    ASSERT_EQ((long)list.tail->data, 42);
}

TEST(list_reverse_multiple_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ck_list_reverse(&list);

    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 3);
    ASSERT_EQ((long)ck_list_get(&list, 1), 2);
    ASSERT_EQ((long)list.tail->data, 1);
}

TEST(list_reverse_circular_maintains_circularity) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ck_list_reverse(&list);

    ASSERT_EQ(list.length, 3);
    ASSERT_EQ((long)list.head->data, 3);
    ASSERT_EQ(list.tail->next, list.head);
}

/* Clear tests. */

TEST(list_clear_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_clear(&list);

    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

TEST(list_clear_with_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ck_list_clear(&list);

    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

TEST(list_clear_circular) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    ck_list_clear(&list);

    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

/* Copy tests. */

TEST(list_copy_empty_list) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    Ck_List copy = ck_list_copy(&list);

    ASSERT_EQ(copy.length, 0);
    ASSERT_NULL(copy.head);
    ASSERT_NULL(copy.tail);
}

TEST(list_copy_with_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);
    ck_list_push(&list, (void*)(long)3);

    Ck_List copy = ck_list_copy(&list);

    ASSERT_EQ(copy.length, 3);
    ASSERT_EQ((long)ck_list_get(&copy, 0), 1);
    ASSERT_EQ((long)ck_list_get(&copy, 1), 2);
    ASSERT_EQ((long)ck_list_get(&copy, 2), 3);

    /* Verify it's a different structure. */
    ASSERT_NEQ(copy.head, list.head);
    ASSERT_NEQ(copy.tail, list.tail);
}

TEST(list_copy_circular) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);

    Ck_List copy = ck_list_copy(&list);

    ASSERT_EQ(copy.length, 2);
    ASSERT_EQ(copy.mode, CK_LIST_MODE_CIRCULAR);
    ASSERT_EQ(copy.tail->next, copy.head);
}

/* Free tests. */

TEST(list_free_empty_list) {
    Ck_List* list = (Ck_List*)ck_memory.alloc(sizeof(Ck_List));
    *list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_free(&list);

    ASSERT_NULL(list);
}

TEST(list_free_with_elements) {
    Ck_List* list = (Ck_List*)ck_memory.alloc(sizeof(Ck_List));
    *list = ck_list_init(NULL, CK_LIST_MODE_LINEAR);

    ck_list_push(list, (void*)(long)1);
    ck_list_push(list, (void*)(long)2);
    ck_list_push(list, (void*)(long)3);

    ck_list_free(&list);

    ASSERT_NULL(list);
}

TEST(list_free_null_pointer) {
    Ck_List* list = NULL;

    ck_list_free(&list);

    ASSERT_NULL(list);
}

/* Edge case: Circular list pop until empty breaks circularity check. */

TEST(list_circular_pop_until_empty_then_push) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);

    /* Pop all elements. */
    ck_list_pop(&list);
    ck_list_pop(&list);

    /* This should work - list is empty, push should recreate circular structure. */
    ck_list_push(&list, (void*)(long)3);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ(list.head, list.tail);
    ASSERT_EQ(list.head->next, list.head); /* FAILS - next is NULL, not circular! */
    ASSERT_EQ(list.tail->next, list.head);
}

/* Another edge case: Circular shift until empty. */

TEST(list_circular_shift_until_empty_then_unshift) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);

    /* Shift all elements. */
    ck_list_shift(&list);
    ck_list_shift(&list);

    /* This should work - list is empty, unshift should recreate circular structure. */
    ck_list_unshift(&list, (void*)(long)3);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ(list.head, list.tail);
    ASSERT_EQ(list.head->next, list.head); /* FAILS - next is NULL, not circular! */
}

/* The bug: pop() and shift() set head/tail to NULL but don't check
   mode when the list becomes empty. When we push/unshift again,
   _ck_list_push_head() expects an empty list but the circular
   linking isn't maintained because the mode isn't rechecked.

   Actually, looking closer - _ck_list_push_head() DOES handle
   circularity. Let me trace through...

   Wait, the real issue: In pop() and shift(), when we hit the
   single element case, we just set head/tail to NULL without
   considering that we need to maintain the empty state properly.

   Actually, re-reading the code... _ck_list_push_head() should
   handle it. Let me find the REAL edge case... */

/* ACTUAL EDGE CASE: Circular mode with copy when list has 1 element. */

TEST(list_circular_copy_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)42);

    Ck_List copy = ck_list_copy(&list);

    ASSERT_EQ(copy.length, 1);
    ASSERT_EQ(copy.head, copy.tail);
    ASSERT_EQ(copy.head->next, copy.head); /* Circularity maintained? */

    /* The bug: In ck_list_copy(), the while loop with sentinel check
       breaks immediately for single element because:
       - first iteration: node == sentinel && !first is false, continue
       - node = node->next (which points back to head/sentinel)
       - second iteration: node == sentinel && !first is TRUE, break
       But we only pushed once, so the copy only has 1 element but
       the circularity is maintained. Actually this should work...

       Let me find a REAL failing edge case... */
}

/* REAL EDGE CASE: Remove from circular list with 2 elements at index 1. */

TEST(list_circular_remove_second_of_two_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_MODE_CIRCULAR);

    ck_list_push(&list, (void*)(long)1);
    ck_list_push(&list, (void*)(long)2);

    /* Remove second element (index 1). */
    void* data = ck_list_remove(&list, 1);

    ASSERT_EQ((long)data, 2);
    ASSERT_EQ(list.length, 1);
    ASSERT_EQ(list.head, list.tail);

    /* BUG: After remove calls pop(), which doesn't maintain circularity
       correctly. The tail->next should point to head for circular mode. */
    ASSERT_EQ(list.tail->next, list.head); /* FAILS - tail->next is NULL! */
}
