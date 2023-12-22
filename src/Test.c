#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>

boolState stateMatchesStringAtIndex(RE *state, char *string, size_t len, int i)
{
    boolState returnState;
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
            returnState.match = (state->table[string[i]] == 1) ? 1 : returnState.match;
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

boolState test(RE **stack, char *string, size_t len)
{
    int i = 0;
    int j = 0;

    RE *current_state = stack[++j];

    while (current_state != (RE *)NULL)
    {
        boolState state;

        switch (current_state->quantifier.type)
        {
        case EXACTLY_ONE:
            state = stateMatchesStringAtIndex(current_state, string, len, i);

            if (state.match == 0)
            {
                boolState returnState;
                returnState.consumed = 0;
                returnState.match = 0;
                return returnState;
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
            i += state.consumed;
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

                i += state.consumed;
            }
            continue;
        case MIN_MAX:
            {
                int matches = 0;
                int max = current_state->quantifier.max;
                int min = current_state->quantifier.min;

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

                    matches++;
                    i += state.consumed;
                }

                if (matches < min || matches > max)
                {
                    boolState returnState;
                    returnState.consumed = 0;
                    returnState.match = 0;
                    return returnState;
                }
            }
            continue;
        default:
            error("Unsupported quantifier", 2);
        }
    }

    boolState returnState;
    returnState.consumed = i;
    returnState.match = 1;
    return returnState;
}

boolState* match(RE** stack, char* string, size_t len) {
    boolState* states = (boolState*) malloc(sizeof(boolState) * DEFAULT_STACK_SIZE * 2);
    if (states == (boolState*) NULL) { return (boolState*) NULL; }

    int stateIndex = 0;
    size_t curIndex = 0;

    while (curIndex < len) {
        boolState state = test(stack, string + curIndex, len - curIndex);

        if (state.match == 0 && state.consumed == 0) {
            curIndex++;
        } else {
            states[stateIndex].match = 1;
            states[stateIndex].consumed = curIndex;

            curIndex += state.consumed;
            states[stateIndex].end = curIndex;

            printf("%.*s Match/End at: %d/%d\n", 
            states[stateIndex].end - states[stateIndex].consumed, 
            string + states[stateIndex].consumed, 
            states[stateIndex].consumed, 
            states[stateIndex].end);

            stateIndex++;

            if (state.consumed == 0) {
                curIndex++;
            }
        }
    }

    return states;
}