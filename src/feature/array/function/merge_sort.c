#ifndef __CKASTAL_DSA_ARRAY_FUNCTION_MERGE_SORT_C__
#define __CKASTAL_DSA_ARRAY_FUNCTION_MERGE_SORT_C__

#include "../../../core/init.c"

void _ck_int_merge_sort_merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void _ck_int_merge_sort_recursive(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        _ck_int_merge_sort_recursive(arr, l, m);
        _ck_int_merge_sort_recursive(arr, m + 1, r);

        _ck_int_merge_sort_merge(arr, l, m, r);
    }
}

/**
 * @brief Public interface for Merge Sort algorithm.
 *
 * Provides a clean, simple interface to sort an integer array
 * using the Merge Sort algorithm.
 *
 * @param arr The array of integers to be sorted.
 * @param n The number of elements in the array.
 */
void ck_int_merge_sort(int arr[], size_t n) {
    _ck_int_merge_sort_recursive(arr, 0, n - 1);
}

#endif /* __CKASTAL_DSA_ARRAY_FUNCTION_MERGE_SORT_C__ */
