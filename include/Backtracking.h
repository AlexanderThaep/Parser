#include <RegExp.h>

#if !defined(BACKTRACK)
#define BACKTRACK

#define BOOLSTACK_SIZE 512

struct BackState
{
    unsigned int index;
    unsigned int stateIndex;
    unsigned short consumed;
    unsigned short backTrackState;
};
// I leave some structures with shorts in order to maintain struct efficiency
// http://www.catb.org/esr/structure-packing/

struct BackStack
{
    unsigned int index;
    unsigned int max;
    struct BackState **states;
};

struct BackStack *createBackStack();
void resetBackStack(struct BackStack *stack);
struct BackStack *pushBackStack(struct BackStack *back_stack, struct BoolState state, unsigned short backTrackState, unsigned int stateIndex);
struct BackState *popBackStack(struct BackStack *back_stack);
struct BackState *backtrack(struct BackStack *back_stack);

#endif