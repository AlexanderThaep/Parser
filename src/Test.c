#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>
#include <Backtracking.h>

BoolState stateMatchesStringAtIndex(RE *state, char *string, size_t len, int i)
{
    BoolState returnState;
    returnState.consumed = 0;
    returnState.match = 0;

    if (i < len)
    {    
        switch (state->type)
        {
        case WILDCARD:
            returnState.consumed = 1;
            returnState.match = 1;
            break;
        case LITERAL:
            returnState.match = (state->data == string[i]) ? 1 : 0;
            returnState.consumed = returnState.match;
            break;
        case GROUP:
            return test(state->child_stack, &(string[i]), len - i);
        case LITERAL_GROUP:
            returnState.match = (state->table[(int)string[i]] == 1) ? 1 : returnState.match;
            returnState.consumed = returnState.match;
            break;
        case OR_GROUP:
            // damn, this works :.)
            {           
                int index = 1;
                RE* stack[3];

                stack[0] = (RE*) NULL;
                stack[2] = (RE*) NULL;

                while (state->child_stack[index] != (RE*) NULL) {
                    stack[1] = state->child_stack[index];
                    returnState = test(stack, &(string)[i], len - i);
                    if (returnState.match == 1) { return returnState; }
                    index++;
                }
            }

            break;
        default:
            error("Unsupported state type", 2);
            break;
        }
        return returnState;
    }
    return returnState;
}

BoolState test(RE **stack, char *string, size_t len)
{
    int i = 0;
    int j = 0;

    BoolState returnState;
    returnState.consumed = 0;
    returnState.match = 0;

    RE *current_state = stack[++j];
    BackStack *back_stack = createBackStack();

    unsigned char stopLaziness = 0;

    while (current_state != (RE *)NULL)
    {
        BoolState state;

        BEGIN:

        current_state = stack[j];

        if (current_state == (RE*) NULL) { break; }
     
        int matches = current_state->matches;
        int max = current_state->quantifier.max;
        int min = current_state->quantifier.min;
        
        switch (current_state->quantifier.type)
        {
        case EXACTLY_ONE:
        case RANGE:
            while (matches < max)
            {
                if (i >= len)
                {
                    break;
                }

                state.end = i;

                if (matches >= min)
                {
                    if (current_state->quantifier.modifier != POSSESSIVE) 
                    { 
                        if (current_state->quantifier.modifier == LAZY && stopLaziness == 0) {
                            pushBackStack(back_stack, state, current_state->quantifier.modifier, j);
                            current_state = stack[++j];
                            goto BEGIN;
                        } else if (stopLaziness == 0) {
                            pushBackStack(back_stack, state, current_state->quantifier.modifier, j);
                        }
                    }
                }

                state = stateMatchesStringAtIndex(current_state, string, len, i);
                if (state.match == 0)
                {
                    break;
                }

                matches++;
                stopLaziness = 0;

                i += state.consumed;
            }

            if (matches < min)
            {
                BackState* backState = backtrack(back_stack);
                if (backState != (BackState*) NULL)
                {
                    i = backState->index;
                    j = backState->stateIndex + 1;

                    if (backState->backTrackState == LAZY) {
                        j--;
                        free(backState);
                        stopLaziness = 1;
                        goto BEGIN;
                    }
                    free(backState);

                    continue;
                } else {

                    resetBackStack(back_stack);
                    return returnState;

                }
            }

            current_state = stack[++j];
            continue;
        default:
            error("Unsupported quantifier", 2);
        }
    }

    returnState.consumed = i;
    returnState.match = 1;

    resetBackStack(back_stack);
    return returnState;
}

BoolState *match(RE **stack, char *string, size_t len)
{
    BoolState *states = (BoolState *)malloc(sizeof(BoolState) * MATCHES_ARRAY);
    if (states == (BoolState *)NULL)
    {
        return (BoolState *)NULL;
    }

    int stateIndex = 0;
    size_t curIndex = 0;

    while (curIndex < len)
    {
        BoolState state = test(stack, string + curIndex, len - curIndex);

        if (state.match == 0 && state.consumed == 0)
        {
            curIndex++;
        }
        else
        {
            states[stateIndex].match = 1;
            states[stateIndex].consumed = curIndex;

            curIndex += state.consumed;
            states[stateIndex].end = curIndex;

            printf("%.*s Start/End at: %d/%d\n",
                   states[stateIndex].end - states[stateIndex].consumed,
                   string + states[stateIndex].consumed,
                   states[stateIndex].consumed,
                   states[stateIndex].end);

            stateIndex++;

            if (state.consumed == 0)
            {
                curIndex++;
            }
        }
    }

    return states;
}