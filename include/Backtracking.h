#include <RegExp.h>

#if !defined(BACKTRACK)
#define BACKTRACK

#define BOOLSTACK_SIZE 512

typedef struct BackState
{
    unsigned int index;
    unsigned int stateIndex;
    unsigned short consumed;
    unsigned short backTrackState;
} BackState;
// I leave some structures with shorts in order to maintain struct efficiency
// http://www.catb.org/esr/structure-packing/

typedef struct BackStack
{
    unsigned int index;
    unsigned int max;
    struct BackState **states;
} BackStack;

BackStack *createBackStack();
void resetBackStack(BackStack *stack);
BackStack *pushBackStack(BackStack *back_stack, BoolState state, unsigned short backTrackState, unsigned int stateIndex);
BackState *popBackStack(BackStack *back_stack);
BackState *backtrack(BackStack *back_stack);

#endif