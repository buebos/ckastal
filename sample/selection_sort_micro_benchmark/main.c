#include <stdio.h>
#include <stdlib.h>
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

int main(void) {
    srand(time(NULL));

    int arr15[15] = {0};
    int arr1000[1000] = {0};
    int arr5000[5000] = {0};
    int arr10000[10000] = {0};

    size_t lengths[] = {15, 1000, 5000, 10000};
    int *arrays[] = {arr15, arr1000, arr5000, arr10000};

    FillStrategy strategies[] = {ORDERED, REVERSED, RANDOM};
    const char *strategy_names[] = {"ORDERED", "REVERSED", "RANDOM"};

    printf("%10s | %10s | %15s\n", "Length", "Strategy", "Time (ms)");
    printf("----------------------------------------\n");

    for (size_t s = 0; s < sizeof(lengths) / sizeof(lengths[0]); s++) {
        for (size_t f = 0; f < sizeof(strategies) / sizeof(strategies[0]); f++) {
            fill(arrays[s], lengths[s], strategies[f]);

            clock_t start = clock();
            ck_int_selection_sort(arrays[s], lengths[s]);
            clock_t end = clock();

            double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;

            printf("%10zu | %10s | %15.6f\n", lengths[s], strategy_names[f], time_taken);
        }
    }

    return 0;
}