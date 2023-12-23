#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>

typedef struct BOOLSTACK {
    unsigned int index;
    unsigned int max;
    boolState** states;
} BOOLSTACK;

BOOLSTACK* createBoolStack();
BOOLSTACK* pushBoolStack(BOOLSTACK* bool_stack, boolState state);
boolState* popBoolStack(BOOLSTACK* bool_stack);
int backtrack(BOOLSTACK* bool_stack);