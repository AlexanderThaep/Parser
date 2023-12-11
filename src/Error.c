#include <stdlib.h>
#include <stdio.h>

void error(char* message, int type) {
    printf("ERROR: %s :::: Type %d\n", message, type);
    exit(EXIT_FAILURE);
}