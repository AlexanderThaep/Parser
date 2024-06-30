#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>
#include <Backtracking.h>
#include <Error.h>

struct BackStack *createBackStack()
{
    struct BackStack *back_stack = (struct BackStack *)malloc(sizeof(struct BackStack));
    if (back_stack == (struct BackStack *)NULL)
    {
        return back_stack;
    }

    back_stack->index = 0;
    back_stack->max = BOOLSTACK_SIZE;
    back_stack->states = (struct BackState **)malloc(sizeof(struct BackState *) * BOOLSTACK_SIZE);

    return back_stack;
};

void resetBackStack(struct BackStack *stack)
{
    if (stack == (struct BackStack *)NULL)
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

struct BackStack *pushBackStack(struct BackStack *back_stack, struct BoolState state, unsigned short backTrackState, unsigned int stateIndex)
{
    if (back_stack->index < back_stack->max)
    {

        struct BackState *newState = (struct BackState *)malloc(sizeof(struct BackState));
        newState->backTrackState = backTrackState;
        newState->consumed = state.consumed;
        newState->index = state.end;
        newState->stateIndex = stateIndex;
        back_stack->states[back_stack->index] = newState;

        back_stack->index++;

        return back_stack;
    }

    error("Bool stack overflow!", FATAL);
    return (struct BackStack *)NULL;
};

struct BackState *popBackStack(struct BackStack *back_stack)
{
    if (back_stack->index > 0)
    {
        back_stack->index--;
    }
    struct BackState *poppedState = back_stack->states[back_stack->index];
    back_stack->states[back_stack->index] = (struct BackState *)NULL;
    return poppedState;
}

struct BackState *backtrack(struct BackStack *back_stack)
{
    struct BackState *poppedState = popBackStack(back_stack);
    return poppedState;
}