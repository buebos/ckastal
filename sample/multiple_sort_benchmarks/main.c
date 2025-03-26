#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/array.c"

typedef enum FillStrategy {
    ORDERED = 0,
    REVERSED,
    RANDOM,
} FillStrategy;

void fill(int arr[], size_t length, FillStrategy strategy) {
    switch (strategy) {
        case ORDERED:
            for (size_t i = 0; i < length; i++) {
                arr[i] = (int)i;
            }

            break;
        case REVERSED:
            for (size_t i = length - 1; i > 0; i--) {
                arr[i] = (int)i;
            }

            break;
        case RANDOM:
            for (size_t i = length - 1; i > 0; i--) {
                arr[i] = rand() % 100;
            }

            break;
    }
}

void custom_sort(int arr[], size_t n) {
}

int main(void) {
    srand(time(NULL));

    int arr_100[100] = {0};
    int arr_1_000[1000] = {0};
    int arr_10_000[10000] = {0};
    int arr_100_0000[100000] = {0};

    size_t lengths[] = {100, 1000, 10000, 100000};
    int *arrays[] = {arr_100, arr_1_000, arr_10_000, arr_100_0000};

    FillStrategy strategies[] = {ORDERED, REVERSED, RANDOM};
    const char *strategy_names[] = {"ORDERED", "REVERSED", "RANDOM"};
    const char *sort_algorithms[] = {"MERGE", "QUICK", "HEAP"};

    printf("%16s | %10s | %10s | %10s | %15s\n", "Benchmark ID", "Algorithm", "Length", "Strategy", "Time (ms)");
    printf("-----------------------------------------------------------------------\n");

    size_t lengths_len = sizeof(lengths) / sizeof(lengths[0]);
    size_t strategies_len = sizeof(strategies) / sizeof(strategies[0]);
    size_t sort_algorithms_len = sizeof(sort_algorithms) / sizeof(sort_algorithms[0]);

    for (size_t s = 0; s < lengths_len; s++) {
        for (size_t f = 0; f < strategies_len; f++) {
            for (size_t a = 0; a < sort_algorithms_len; a++) {
                char *target_algorithm = ((char *)sort_algorithms[a]);

                fill(arrays[s], lengths[s], strategies[f]);

                clock_t start;

                if (strcmp(target_algorithm, "MERGE")) {
                    start = clock();
                    ck_int_merge_sort(arrays[s], lengths[s]);
                } else if (strcmp(target_algorithm, "QUICK")) {
                    start = clock();
                    ck_int_quick_sort(arrays[s], lengths[s]);
                } else if (strcmp(target_algorithm, "HEAP")) {
                    start = clock();
                    ck_int_heap_sort(arrays[s], lengths[s]);
                }

                clock_t end = clock();

                double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

                printf("%16zu | %10s | %10zu | %10s | %15.6f\n", (s * (strategies_len * sort_algorithms_len) + f * sort_algorithms_len + a) + 1, target_algorithm, lengths[s], strategy_names[f], time_taken);
            }
        }

        printf("-----------------------------------------------------------------------\n");
    }

    return 0;
}