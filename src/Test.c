#include <stdlib.h>
#include <stdio.h>
#include <RegTypes.h>

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

        switch (current_state->quantifier)
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

        default:
            error("Unsupported quantifier", 2);
        }
    }

    boolState returnState;
    returnState.consumed = i;
    returnState.match = 1;
    return returnState;
}