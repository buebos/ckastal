#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "../../../include/array.c"
#include "../../../vendor/clogg/src/clogg.c"

CK_ARRAY_DECLARE_BUBBLE_SORT_FN(int, int_bubble_sort, {
    should_swap = *a > *b;
});

void print_array(int* array, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main() {
    int arr1[] = {5, 2, 9, 1, 5, 6};
    size_t len1 = sizeof(arr1) / sizeof(arr1[0]);
    int_bubble_sort(arr1, len1);

    for (size_t i = 1; i < len1; i++) {
        assert(arr1[i - 1] <= arr1[i]);
    }

    int arr2[] = {1, 2, 3, 4, 5, 6};
    size_t len2 = sizeof(arr2) / sizeof(arr2[0]);
    int_bubble_sort(arr2, len2);
    for (size_t i = 1; i < len2; i++) {
        assert(arr2[i - 1] <= arr2[i]);
    }

    int arr3[] = {5, 2, 9, 2, 5, 6};
    size_t len3 = sizeof(arr3) / sizeof(arr3[0]);
    int_bubble_sort(arr3, len3);
    for (size_t i = 1; i < len3; i++) assert(arr3[i - 1] <= arr3[i]);

    int arr4[] = {};
    size_t len4 = sizeof(arr4) / sizeof(arr4[0]);
    int_bubble_sort(arr4, len4);

    cloggln(CLOGG_INFO, "All tests passed");

    return 0;
}
