#ifndef __CKASTAL_DSA_ARRAY_FUNCTION_HEAP_SORT_C__
#define __CKASTAL_DSA_ARRAY_FUNCTION_HEAP_SORT_C__

#include "../../../core/init.c"

/**
 * Critical operation in Heap Sort that ensures the max-heap property
 * is maintained. It assumes that the left and right subtrees are already max-heaps,
 * and corrects the root element's position to satisfy the max-heap condition.
 */
void _ck_int_heap_sort_heapify(int arr[], size_t n, size_t root) {
    size_t largest = root;
    size_t left = 2 * root + 1;
    size_t right = 2 * root + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != root) {
        int temp = arr[root];
        arr[root] = arr[largest];
        arr[largest] = temp;

        _ck_int_heap_sort_heapify(arr, n, largest);
    }
}

/**
 * @brief Sorts an array of integers using the Heap Sort algorithm.
 *
 * Heap Sort is an efficient, comparison-based sorting algorithm that uses
 * a binary heap data structure. It works by first building a max-heap from
 * the input array and then repeatedly extracting the maximum element.
 *
 * @param arr The array of integers to be sorted.
 * @param n The number of elements in the array.
 *
 * @details
 * - **Sorting Strategy:**
 *   1. Build a max-heap from the input array
 *   2. Repeatedly extract the maximum element and place it at the end
 *   3. Reduce the heap size and heapify the root
 *
 * - **Time Complexity:**
 *   - Worst Case: O(n log n)
 *   - Average Case: O(n log n)
 *   - Best Case: O(n log n)
 *
 * - **Space Complexity:** O(1) - in-place sorting
 *
 * - **Characteristics:**
 *   - Not stable (may change relative order of equal elements)
 *   - In-place sorting algorithm
 *   - Efficient for large datasets
 *
 * @note Modifies the original array in-place during sorting.
 */
void ck_int_heap_sort(int arr[], size_t n) {
    /**
     * Build max heap and rearrange array
     */
    for (int i = n / 2 - 1; i >= 0; i--) {
        _ck_int_heap_sort_heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        _ck_int_heap_sort_heapify(arr, i, 0);
    }
}

#endif /* __CKASTAL_DSA_ARRAY_FUNCTION_HEAP_SORT_C__ */