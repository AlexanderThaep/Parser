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

    while (current_state != (RE *)NULL)
    {
        BoolState state;

        switch (current_state->quantifier.type)
        {
        case EXACTLY_ONE:
            state = stateMatchesStringAtIndex(current_state, string, len, i);

            if (state.match == 0)
            {
                BackState* backState = backtrack(back_stack);
                if (backState == (BackState*) NULL 
                || backState->consumed < 1)
                {
                    goto END;
                }
                i = backState->index;
                if (backState->backTrackState == LAZY) { i++; current_state = stack[--j]; }

                free(backState);
                continue;
            }

            i += state.consumed;
            current_state = stack[++j];

            continue;
        case ZERO_OR_ONE:
            if (i >= len)
            {
                current_state = stack[++j];
                continue;
            }

            state = stateMatchesStringAtIndex(current_state, string, len, i);
            state.end = i;
            i += state.consumed;

            if (current_state->quantifier.modifier != POSSESSIVE) 
            { pushBackStack(back_stack, state, current_state->quantifier.modifier); }

            current_state = stack[++j];
            continue;
        case ZERO_OR_MORE:
            while (1)
            {
                if (i >= len)
                {
                    current_state = stack[++j];
                    break;
                }

                state = stateMatchesStringAtIndex(current_state, string, len, i);
                if (state.match == 0 || state.consumed == 0)
                {
                    current_state = stack[++j];
                    break;
                }

                state.end = i;
                i += state.consumed;
                
                if (current_state->quantifier.modifier != POSSESSIVE) 
                { pushBackStack(back_stack, state, current_state->quantifier.modifier); }

                if (current_state->quantifier.modifier == LAZY) {
                    current_state = stack[++j];
                    break;
                }
            }
            continue;
        case MIN_MAX:
            {
                int matches = 0;
                int max = current_state->quantifier.max;
                int min = current_state->quantifier.min;

                while (matches < max)
                {
                    if (i >= len)
                    {
                        current_state = stack[++j];
                        break;
                    }

                    state = stateMatchesStringAtIndex(current_state, string, len, i);
                    if (state.match == 0 || state.consumed == 0)
                    {
                        current_state = stack[++j];
                        break;
                    }

                    matches++;

                    state.end = i;
                    i += state.consumed;

                    if (matches > min)
                    {
                        if (current_state->quantifier.modifier != POSSESSIVE) 
                        { pushBackStack(back_stack, state, current_state->quantifier.modifier); }

                        if (current_state->quantifier.modifier == LAZY) {
                            current_state = stack[++j];
                            break;
                        }
                    }
                }

                if (matches < min)
                {
                    BackState* backState = backtrack(back_stack);
                    if (backState == (BackState*) NULL 
                    || backState->consumed < 1)
                    {
                        goto END;
                    }
                    i = backState->index;
                    if (backState->backTrackState == LAZY) { i++; current_state = stack[--j]; }

                    free(backState);
                    continue;
                }
                current_state = stack[++j];
            }
            continue;
        default:
            error("Unsupported quantifier", 2);
        }
    }

    returnState.consumed = i;
    returnState.match = 1;
END:
    resetBackStack(back_stack);
    return returnState;
}

BoolState *match(RE **stack, char *string, size_t len)
{
    BoolState *states = (BoolState *)malloc(sizeof(BoolState) * DEFAULT_STACK_SIZE * 2);
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