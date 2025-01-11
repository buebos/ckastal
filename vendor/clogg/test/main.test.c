#include "../include/clogg.h"

int main(int argc, char** argv) {
    for (int i = 0; i < CLOGG_LOG_LEVEL_COUNT; i++) {
        cloggln(i, "%s %s", CLOGG_PREFIXES[i], "message");
    }

    printf("\n");
    cloggln(CLOGG_INFO, "Logging with variable arguments: ");

    for (int i = 0; i < CLOGG_LOG_LEVEL_COUNT; i++) {
        cloggln(i, "argc = %d, argv[0] = %s", argc, argv[0]);
    }

    printf("\n");

    cloggdev("Development mode is on, argc = %d\n", argc);

    return 0;
}