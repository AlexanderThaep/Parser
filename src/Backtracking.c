#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>
#include <Backtracking.h>

BackStack* createBackStack() {
    BackStack* back_stack = (BackStack*) malloc(sizeof(BackStack));
    if (back_stack == (BackStack*) NULL) { return back_stack; }

    back_stack->index = 0;
    back_stack->max = BOOLSTACK_SIZE;
    back_stack->states = (BackState**) malloc(sizeof(BackState*) * BOOLSTACK_SIZE);

    return back_stack;
};

void resetBackStack(BackStack* stack) {
    if (stack == (BackStack*) NULL) {
        return;
    }
    for (int i = 0; i <= stack->index; i++) {
        free(stack->states[i]);
    }
    free(stack);
    return;
}

BackStack* pushBackStack(BackStack* back_stack, BoolState state, unsigned short backTrackState) {
    if (back_stack->index < back_stack->max) {

        back_stack->index++;

        BackState* newState = (BackState*) malloc(sizeof(BackState));
        newState->backTrackState = backTrackState;
        newState->consumed = state.consumed;
        newState->index = state.end;
        newState->matches = 0;
        back_stack->states[back_stack->index] = newState;

        return back_stack;
    }
    error("Bool stack overflow!", 2);

    return (BackStack*) NULL;
};

BackState* popBackStack(BackStack* back_stack) {
    if (back_stack->index > 0) {
        BackState* poppedState = back_stack->states[back_stack->index];

        back_stack->index--;
        return poppedState;
    }

    return (BackState*) NULL;
}

BackState* backtrack(BackStack* back_stack) {
    BackState* poppedState = popBackStack(back_stack);
    return poppedState;
}