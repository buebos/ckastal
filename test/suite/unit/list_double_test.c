#include <stdlib.h>

/** */
#include "../../../include/list_double.c"
#include "../../../vendor/ckester/include/index.c"

TEST(list_double_linear_push) {
    int list_data_array[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    Ck_ListDouble list = ck_listdouble_init((Ck_ListDoubleInitParams){
        .mode = CK_LIST_DOUBLE_MODE_LINEAR,
        .type = (Ck_DataType){.size = sizeof(int), 0},
    });

    for (size_t i = 0; i < 8; i++) {
        ck_listdouble_push(&list, &list_data_array[i]);
        ASSERT_EQ(ck_listdouble_get(&list, i), &list_data_array[i]);
    }
}

TEST(list_double_circular_push) {
    int list_data_array[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    Ck_ListDouble list = ck_listdouble_init((Ck_ListDoubleInitParams){
        .mode = CK_LIST_DOUBLE_MODE_CIRCULAR,
        .type = (Ck_DataType){.size = sizeof(int), 0},
    });

    for (size_t i = 0; i < 8; i++) {
        ck_listdouble_push(&list, &list_data_array[i]);
        ASSERT_EQ(ck_listdouble_get(&list, i), &list_data_array[i]);
    }
}
