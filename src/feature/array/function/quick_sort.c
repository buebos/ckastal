#ifndef __CKASTAL_DSA_ARRAY_FUNCTION_QUICK_SORT_C__
#define __CKASTAL_DSA_ARRAY_FUNCTION_QUICK_SORT_C__

#include "../../../core/init.c"

/**
 * @brief Selects a pivot and partitions the array around it.
 *
 * This function is a core component of the Quick Sort algorithm,
 * responsible for choosing a pivot and rearranging array elements
 * such that elements smaller than the pivot are moved to its left,
 * and elements larger are moved to its right.
 *
 * @param arr The array to be partitioned.
 * @param low Starting index of the array or subarray.
 * @param high Ending index of the array or subarray.
 *
 * @return The index of the pivot after partitioning.
 *
 * @details
 * - **Pivot Selection Strategy:**
 *   Uses the last element as the pivot.
 * - **Partitioning Process:**
 *   1. Choose the rightmost element as pivot
 *   2. Place pivot in its correct position
 *   3. Move smaller elements to the left of pivot
 *   4. Move larger elements to the right of pivot
 *
 * - **Time Complexity:** O(n), where n is the number of elements
 * - **Space Complexity:** O(1) - in-place partitioning
 *
 * @note This implementation uses Lomuto partition scheme.
 */
size_t _ck_int_quick_sort_partition(
    int *arr,
    size_t low,
    size_t high

) {
    int pivot = arr[high];

    size_t i = low - 1;

    for (size_t j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;

            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

/**
 * @brief Recursively implements the Quick Sort algorithm.
 *
 * Divides the array into smaller subarrays, sorts them,
 * and combines the results.
 *
 * @param arr The array to be sorted.
 * @param low Starting index of the array or subarray.
 * @param high Ending index of the array or subarray.
 *
 * @details
 * - **Sorting Strategy:**
 *   1. Choose a pivot element
 *   2. Partition the array around the pivot
 *   3. Recursively sort the subarrays
 *
 * - **Divide and Conquer Approach:**
 *   - Breaks down the sorting problem into smaller subproblems
 *   - Solves subproblems recursively
 *   - Combines solutions through partitioning
 *
 * - **Time Complexity:**
 *   - Worst Case: O(n²) - when array is already sorted
 *   - Average Case: O(n log n)
 *   - Best Case: O(n log n)
 *
 * - **Space Complexity:**
 *   - Worst Case: O(n) due to recursive call stack
 *   - Best Case: O(log n)
 *
 * - **Characteristics:**
 *   - Not a stable sorting algorithm
 *   - In-place sorting
 *   - Efficient for large datasets
 *   - Performance depends on pivot selection
 *
 * @note Modifies the original array in-place during sorting.
 */
void _ck_int_quick_sort_recursive(
    int *arr,
    size_t low,
    size_t high

) {
    if (low >= high) {
        return;
    }

    size_t pivot_index = _ck_int_quick_sort_partition(arr, low, high);

    if (pivot_index > low) {
        _ck_int_quick_sort_recursive(arr, low, pivot_index - 1);
    }

    if (pivot_index < high) {
        _ck_int_quick_sort_recursive(arr, pivot_index + 1, high);
    }
}

/**
 * @brief Public interface for Quick Sort algorithm.
 *
 * Provides a clean, simple interface to sort an integer array
 * using the Quick Sort algorithm.
 *
 * @param arr The array of integers to be sorted.
 * @param n The number of elements in the array.
 */
void ck_int_quick_sort(int arr[], size_t n) {
    if (n <= 1) {
        return;
    }

    _ck_int_quick_sort_recursive(arr, 0, n - 1);
}

#endif /* __CKASTAL_DSA_ARRAY_FUNCTION_QUICK_SORT_C__ */