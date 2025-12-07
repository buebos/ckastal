#include "../../../include/array.c"

#include "../../../vendor/ckester/include/index.c"

CK_ARRAY_DECLARE_BUBBLE_SORT_FN(int, int_bubble_sort, {
    should_swap = *a > *b;
});

TEST(bubble_sort_random_array) {
    int arr[] = {5, 2, 9, 1, 5, 6};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    int_bubble_sort(arr, len);

    for (size_t i = 1; i < len; i++) {
        ASSERT_TRUE(arr[i - 1] <= arr[i]);
    }
}

TEST(bubble_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    int_bubble_sort(arr, len);

    for (size_t i = 1; i < len; i++) {
        ASSERT_TRUE(arr[i - 1] <= arr[i]);
    }
}

TEST(bubble_sort_with_duplicates) {
    int arr[] = {5, 2, 9, 2, 5, 6};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    int_bubble_sort(arr, len);

    for (size_t i = 1; i < len; i++) {
        ASSERT_TRUE(arr[i - 1] <= arr[i]);
    }
}

TEST(bubble_sort_empty_array) {
    int arr[] = {};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    int_bubble_sort(arr, len);

    /* Empty array should not crash. */
    ASSERT_EQ(len, 0);
}

TEST(bubble_sort_single_element) {
    int arr[] = {42};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    int_bubble_sort(arr, len);

    ASSERT_EQ(arr[0], 42);
    ASSERT_EQ(len, 1);
}

TEST(bubble_sort_reverse_sorted) {
    int arr[] = {6, 5, 4, 3, 2, 1};
    size_t len = sizeof(arr) / sizeof(arr[0]);
    int_bubble_sort(arr, len);

    for (size_t i = 1; i < len; i++) {
        ASSERT_TRUE(arr[i - 1] <= arr[i]);
    }

    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[len - 1], 6);
}
