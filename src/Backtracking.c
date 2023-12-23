#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>

BOOLSTACK* createBoolStack() {
    BOOLSTACK* bool_stack = (BOOLSTACK*) malloc(sizeof(BOOLSTACK));
    if (bool_stack == (BOOLSTACK*) NULL) { return bool_stack; }

    bool_stack->index = 0;
    bool_stack->max = BOOLSTACK_SIZE;
    bool_stack->states = (boolState**) malloc(sizeof(boolState*) * BOOLSTACK_SIZE);

    return bool_stack;
};

BOOLSTACK* pushBoolStack(BOOLSTACK* bool_stack, boolState state) {
    bool_stack->index++;

    if (bool_stack->index < bool_stack->max) {

        boolState* newState = (boolState*) malloc(sizeof(boolState));
        newState->match = state.match;
        newState->consumed = state.consumed;
        newState->end = state.end;
        bool_stack->states[bool_stack->index] = newState;

        return bool_stack;
    }
    error("Bool stack overflow!", 2);

    return (BOOLSTACK*) NULL;
};

boolState* popBoolStack(BOOLSTACK* bool_stack) {
    if (bool_stack->index > 0) {
        boolState* poppedState = bool_stack->states[bool_stack->index];

        bool_stack->index--;
        return poppedState;
    }

    return (boolState*) NULL;
}

int backtrack(BOOLSTACK* bool_stack) {
    boolState* poppedState = popBoolStack(bool_stack);
    if (poppedState != (boolState*) NULL && poppedState->consumed > 0) {
        int index = poppedState->end;

        free(poppedState);
        return index;
    }
    return -1;
}