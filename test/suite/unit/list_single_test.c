#include <stdlib.h>

#include "../../../include/linked_list.c"
#include "../../ckastal_test_runner.c"

TEST(list_push_and_unshift_100_elements) {
    Ck_List list = ck_list_init(NULL, CK_LIST_LINEAR);

    for (int i = 1; i <= 100; i++) {
        ck_list_push(&list, (void*)(long)i);
        ck_list_unshift(&list, (void*)(long)i);

        ASSERT_EQ(list.length, i * 2);
        ASSERT_EQ((long)list.tail->data, i);
        ASSERT_EQ((long)list.head->data, i);
    }
}

TEST(list_push_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_LINEAR);

    ck_list_push(&list, (void*)(long)42);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ((long)list.head->data, 42);
    ASSERT_EQ((long)list.tail->data, 42);
    ASSERT_EQ(list.head, list.tail);
}

TEST(list_unshift_single_element) {
    Ck_List list = ck_list_init(NULL, CK_LIST_LINEAR);

    ck_list_unshift(&list, (void*)(long)42);

    ASSERT_EQ(list.length, 1);
    ASSERT_EQ((long)list.head->data, 42);
    ASSERT_EQ((long)list.tail->data, 42);
    ASSERT_EQ(list.head, list.tail);
}

TEST(list_empty_initialization) {
    Ck_List list = ck_list_init(NULL, CK_LIST_LINEAR);

    ASSERT_EQ(list.length, 0);
    ASSERT_NULL(list.head);
    ASSERT_NULL(list.tail);
}

TEST(list_alternating_push_unshift) {
    Ck_List list = ck_list_init(NULL, CK_LIST_LINEAR);

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
