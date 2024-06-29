#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>
#include <Backtracking.h>
#include <Error.h>

BackStack *createBackStack()
{
    BackStack *back_stack = (BackStack *)malloc(sizeof(BackStack));
    if (back_stack == (BackStack *)NULL)
    {
        return back_stack;
    }

    back_stack->index = 0;
    back_stack->max = BOOLSTACK_SIZE;
    back_stack->states = (BackState **)malloc(sizeof(BackState *) * BOOLSTACK_SIZE);

    return back_stack;
};

void resetBackStack(BackStack *stack)
{
    if (stack == (BackStack *)NULL)
    {
        return;
    }
    for (int i = 0; i < stack->max; i++)
    {
        free(stack->states[i]);
    }
    free(stack);
    return;
}

BackStack *pushBackStack(BackStack *back_stack, BoolState state, unsigned short backTrackState, unsigned int stateIndex)
{
    if (back_stack->index < back_stack->max)
    {

        BackState *newState = (BackState *)malloc(sizeof(BackState));
        newState->backTrackState = backTrackState;
        newState->consumed = state.consumed;
        newState->index = state.end;
        newState->stateIndex = stateIndex;
        back_stack->states[back_stack->index] = newState;

        back_stack->index++;

        return back_stack;
    }

    error("Bool stack overflow!", FATAL);
    return (BackStack *)NULL;
};

BackState *popBackStack(BackStack *back_stack)
{
    if (back_stack->index > 0)
    {
        back_stack->index--;
    }
    BackState *poppedState = back_stack->states[back_stack->index];
    back_stack->states[back_stack->index] = (BackState *)NULL;
    return poppedState;
}

BackState *backtrack(BackStack *back_stack)
{
    BackState *poppedState = popBackStack(back_stack);
    return poppedState;
}