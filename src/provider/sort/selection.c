#ifndef __CKASTAL_SORT_SELECTION_C__
#define __CKASTAL_SORT_SELECTION_C__

#include "../../core/init.c"

void ck_int_selection_sort(int arr[], size_t length) {
    for (size_t i = 0; i < length - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < length; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        int temp = arr[i];

        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

#endif /* __CKASTAL_SORT_SELECTION_C__ */
