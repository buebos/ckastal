#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/init.c"
#include "../../include/input.c"

typedef struct City {
    int id;
    char* name;
    bool visited;
} City;

bool is_symmetric_matrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int compute_route_distance(int** distances, int* route, int n) {
    int total_distance = 0;

    for (int i = 0; i < n - 1; i++) {
        total_distance += distances[route[i]][route[i + 1]];
    }

    total_distance += distances[route[n - 1]][route[0]];

    return total_distance;
}

void permute(int** distances, int* route, int* best_route, int* min_distance, int start, int n) {
    if (start == n - 1) {
        int current_distance = compute_route_distance(distances, route, n);

        if (current_distance < *min_distance) {
            *min_distance = current_distance;

            for (int i = 0; i < n; i++) {
                best_route[i] = route[i];
            }
        }

        return;
    }

    for (int i = start; i < n; i++) {
        swap(&route[start], &route[i]);

        permute(distances, route, best_route, min_distance, start + 1, n);

        swap(&route[start], &route[i]);
    }
}

int* tsp_brute_force(int** distances, int n) {
    int* route = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        route[i] = i;
    }

    int* best_route = malloc(n * sizeof(int));
    int min_distance = INT_MAX;

    permute(distances, route, best_route, &min_distance, 1, n);

    free(route);

    return best_route;
}

int main(void) {
    int n;
    int** distances;

    printf("Enter the number of cities: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("The number of cities must be greater than 0.\n");

        return 1;
    }

    distances = (int**)ck_memory.alloc(n * sizeof(int*));

    if (!distances) {
        printf("Memory allocation error.\n");

        return 1;
    }

    for (int i = 0; i < n; i++) {
        distances[i] = (int*)ck_memory.alloc(n * sizeof(int));

        if (!distances[i]) {
            printf("Memory allocation error.\n");

            for (int j = 0; j < i; j++) {
                ck_memory.free(distances[j]);
            }

            ck_memory.free(distances);
            return 1;
        }
    }

    printf("Enter the distance matrix:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char buffer[16] = {0};

            ck_input(
                buffer,
                16,
                (Ck_InputParams){
                    .prompt = "",
                    .on_after_prompt = NULL,
                    .on_before_prompt = NULL,
                    .validator = NULL,
                    .skip_end_new_line = true,
                }

            );

            distances[i][j] = atoi(buffer);

            printf(" ");
        }

        printf("\n");
    }

    if (!is_symmetric_matrix(distances, n)) {
        printf("Error: The distance matrix is not symmetric.\n");

        for (int i = 0; i < n; i++) {
            ck_memory.free(distances[i]);
        }

        ck_memory.free(distances);

        return 1;
    }

    int* best_route = tsp_brute_force(distances, n);

    printf("Best route: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", best_route[i]);
    }
    printf("%d\n", best_route[0]);

    int total_distance = compute_route_distance(distances, best_route, n);
    printf("Total distance: %d\n", total_distance);

    ck_memory.free(best_route);
    for (int i = 0; i < n; i++) {
        ck_memory.free(distances[i]);
    }
    ck_memory.free(distances);

    return 0;
}
