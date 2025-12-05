#ifndef __CKASTAL_DSA_ARRAY_FUNCTION_RADIX_SORT_C__
#define __CKASTAL_DSA_ARRAY_FUNCTION_RADIX_SORT_C__

#include "../../../core/init.c"

int _ck_int_radix_sort_get_max(int arr[], size_t n) {
    /**
     * Find the maximum value in the array.
     * This determines the highest place value (units, tens, hundreds, etc.)
     * that needs to be considered when sorting.
     */
    int max = arr[0];
    for (size_t i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

/**
 * @brief Sorts an array of integers using Radix Sort.
 *
 * Radix Sort is a non-comparative, stable sorting algorithm that sorts numbers
 * digit by digit, from the least significant digit (LSD) to the most significant
 * digit (MSD). This implementation uses Counting Sort as a subroutine to sort
 * the numbers at each digit level.
 *
 * @param arr The array of integers to be sorted.
 * @param n The number of elements in the array.
 *
 * @details
 * - **Concept:** Radix Sort relies on the idea that numbers can be sorted by
 * processing their digits sequentially.
 * - **Stability:** Since the sorting at each digit place maintains the relative
 * order of elements with the same value, Radix Sort is a stable sorting
 * algorithm.
 * - **Time Complexity:** O(d * (n + k)), where:
 *   - `d` is the number of digits in the maximum number.
 *   - `n` is the number of elements in the array.
 *   - `k` (10 in this case) is the range of digits (0-9).
 * - **Space Complexity:** O(n) due to the auxiliary array used for sorting.
 * - **Best suited for:** Sorting integers or fixed-length strings, especially
 * when the maximum value has a limited number of digits.
 *
 * @note This implementation assumes all numbers are non-negative. Handling
 * negative numbers would require additional preprocessing.
 */
void ck_int_radix_sort(int arr[], size_t n) {
    /**
     * Determine the largest number in the array.
     * This establishes the maximum number of digits we need to process.
     */
    int max = _ck_int_radix_sort_get_max(arr, n);

    /**
     * Iterate through each digit place (units, tens, hundreds, etc.)
     * until reaching the highest digit place in the maximum number.
     */
    for (int exp = 1; max / exp > 0; exp *= 10) {
        /**
         * Create a temporary array to store sorted values.
         * This avoids modifying the original array while sorting in place.
         */
        int* output = ck_memory.alloc(sizeof(int) * n);

        /**
         * Create a count array where each index corresponds to a digit (0-9).
         * The value at each index represents how many times that digit appears
         * in the current digit place of the numbers being sorted.
         */
        int count[10] = {0};

        /**
         * Count occurrences of each digit at the current place value.
         * This helps in determining the position of each number in the sorted order.
         */
        for (size_t i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % 10;
            count[digit]++;
        }

        /**
         * Transform the count array so that each index now contains
         * the cumulative sum of previous counts. This allows determining
         * the exact position of each element in the sorted order.
         */
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        /**
         * Build the sorted output array.
         * Iterate in reverse to maintain the stable sorting property,
         * ensuring that numbers with the same digit maintain their relative order.
         */
        for (int i = n - 1; i >= 0; i--) {
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }

        /**
         * Copy the sorted values from the output array back into the original array.
         * This prepares the array for the next digit place iteration.
         */
        for (size_t i = 0; i < n; i++) {
            arr[i] = output[i];
        }
    }
}

#endif /* __CKASTAL_DSA_ARRAY_FUNCTION_RADIX_SORT_C__ */
