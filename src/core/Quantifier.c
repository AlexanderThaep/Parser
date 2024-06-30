#include <stdio.h>
#include <stdlib.h>
#include <RegExp.h>

int feedQuantifier(struct RE *regular_expression, size_t i, char *re, size_t len)
{
    unsigned int max = 0;
    unsigned int min = 0;

    unsigned int *place = &min;

    short accessedMin = 0;
    short accessedMax = 0;

    while (i < len)
    {
        if (re[i] <= ASCII_9 && re[i] >= ASCII_0)
        {
            (*place) = ((*place) * 10) + (re[i] - 48);
            if (place == &min)
            {
                accessedMin = 1;
            }
            else
            {
                accessedMax = 1;
            }
        }
        else if (re[i] == ASCII_COMMA)
        {
            place = &max;
        }
        else if (re[i] == '}')
        {
            break;
        }
        i++;
    }

    if (accessedMin == 0 && accessedMax == 0)
    {
        max = INT32_MAX;
    }
    if (accessedMax == 0 && place == &max)
    {
        max = INT32_MAX;
    }
    if (max < min)
    {
        max = min;
    }

    regular_expression->quantifier.type = RANGE;
    regular_expression->quantifier.max = max;
    regular_expression->quantifier.min = min;
    regular_expression->quantifier.modifier = GREEDY;

    return i;
}