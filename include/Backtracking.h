#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>

#if !defined(BACKTRACK)
#define BACKTRACK

#define BOOLSTACK_SIZE 512

typedef struct BackState {
    unsigned int index;
    unsigned int consumed;
    unsigned int matches;
    unsigned short backTrackState;
} BackState;

typedef struct BackStack {
    unsigned int index;
    unsigned int max;
    struct BackState** states;
} BackStack;

BackStack* createBackStack();
void resetBackStack(BackStack* stack);
BackStack* pushBackStack(BackStack* back_stack, BoolState state, unsigned short backTrackState);
BackState* popBackStack(BackStack* back_stack);
BackState* backtrack(BackStack* back_stack);

#endif