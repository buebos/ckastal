#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/input.c"

#define INPUT_BUFFER_SIZE 256

typedef struct {
    char* gene;
    int fitness;
} Individual;

char* generate_random_individual(int length) {
    char* individual = calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; i++) {
        int r = rand() % 52;
        if (r < 26) {
            individual[i] = 'a' + r;
        } else {
            individual[i] = 'A' + (r - 26);
        }
    }
    individual[length] = '\0';
    return individual;
}

void initialize_population(Individual* population, int pop_size, int target_length) {
    for (int i = 0; i < pop_size; i++) {
        population[i].gene = generate_random_individual(target_length);
        population[i].fitness = 0;
    }
}

int calculate_fitness(char* individual, char* target) {
    int fitness = 0;
    for (int i = 0; i < strlen(target); i++) {
        if (individual[i] == target[i]) {
            fitness++;
        }
    }
    return fitness;
}

void evaluate_population(Individual* population, int pop_size, char* target) {
    for (int i = 0; i < pop_size; i++) {
        population[i].fitness = calculate_fitness(population[i].gene, target);
    }
}

int compare_fitness(const void* a, const void* b) {
    Individual* indA = (Individual*)a;
    Individual* indB = (Individual*)b;
    return indB->fitness - indA->fitness;
}

void selection_sort(Individual* population, int pop_size) {
    for (int i = 0; i < pop_size - 1; i++) {
        for (int j = 0; j < pop_size - i - 1; j++) {
            if (population[j].fitness < population[j + 1].fitness) {
                Individual temp = population[j];
                population[j] = population[j + 1];
                population[j + 1] = temp;
            }
        }
    }
}

void crossover(Individual* population, int pop_size, double crossover_rate, int target_length) {
    for (int i = 0; i < pop_size; i += 2) {
        if (i + 1 < pop_size && (double)rand() / RAND_MAX < crossover_rate) {
            int crossover_point = rand() % target_length;

            char* offspring1 = calloc(target_length + 1, sizeof(char));
            char* offspring2 = calloc(target_length + 1, sizeof(char));

            strncpy(offspring1, population[i].gene, crossover_point);
            strncpy(offspring2, population[i + 1].gene, crossover_point);

            strcpy(offspring1 + crossover_point, population[i + 1].gene + crossover_point);
            strcpy(offspring2 + crossover_point, population[i].gene + crossover_point);

            free(population[pop_size / 2 + i].gene);
            free(population[pop_size / 2 + i + 1].gene);

            population[pop_size / 2 + i].gene = offspring1;
            if (i + 1 < pop_size / 2)
                population[pop_size / 2 + i + 1].gene = offspring2;
            else
                free(offspring2);
        }
    }
}

void mutation(Individual* population, int pop_size, double mutation_rate, int target_length) {
    for (int i = 0; i < pop_size; i++) {
        for (int j = 0; j < target_length; j++) {
            if ((double)rand() / RAND_MAX < mutation_rate) {
                int r = rand() % 52;
                if (r < 26)
                    population[i].gene[j] = 'a' + r;
                else
                    population[i].gene[j] = 'A' + (r - 26);
            }
        }
    }
}

void free_population(Individual* population, int pop_size) {
    for (int i = 0; i < pop_size; i++) {
        free(population[i].gene);
    }
    free(population);
}

int main() {
    srand(time(NULL));

    int pop_size = 100;
    double mutation_rate = 0.01;
    double crossover_rate = 0.7;
    int max_generations = 500;

    char target[INPUT_BUFFER_SIZE] = {0};

    ck_input(
        target,
        INPUT_BUFFER_SIZE,
        (Ck_InputParams){
            .prompt = "Enter target password: ",
            .validator = NULL,
            .use_max_length = false,
            0,
        }

    );

    int target_length = strlen(target);
    Individual* population = calloc(pop_size * sizeof(Individual), sizeof(char));

    initialize_population(population, pop_size, target_length);

    int generation = 1;
    int found = 0;

    while (generation <= max_generations && !found) {
        evaluate_population(population, pop_size, target);
        selection_sort(population, pop_size);

        printf("Generation %d - Best: %s (Fitness: %d/%d)\n",
               generation, population[0].gene, population[0].fitness, target_length);

        if (population[0].fitness == target_length) {
            found = 1;
            printf("Target password found in generation %d!\n", generation);
            break;
        }

        crossover(population, pop_size, crossover_rate, target_length);
        mutation(population, pop_size, mutation_rate, target_length);
        generation++;
    }

    if (!found) {
        printf("Maximum generations reached without finding exact match.\n");
        printf("Best solution: %s (Fitness: %d/%d)\n",
               population[0].gene, population[0].fitness, target_length);
    }

    free_population(population, pop_size);
    return 0;
}
