#include <stdio.h>
#include <stdlib.h>
#include <RegExp.h>
#include <Error.h>

int feedQuantifier(struct RE *regular_expression, size_t i, char *re, size_t len)
{
    unsigned int max = 0;
    unsigned int min = 0;

    unsigned int *place = &min;

    short accessedMin = 0;
    short accessedMax = 0;

    while (i < len)
    {
        switch (re[i]) 
        {
        case '9':
        case '8':
        case '7':
        case '6':
        case '5':
        case '4':
        case '3':
        case '2':
        case '1':
        case '0':
            (*place) = ((*place) * 10) + (re[i] - 48);
            if (place == &min)
            {
                accessedMin = 1;
            }
            else
            {
                accessedMax = 1;
            }
            break;
        case ',':
            place = &max;
            break;
        case '}':
            break;
        default:
            error("Symbol not allowed within {}", FATAL);
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

    // Funnily enough, by standards, using {} actually makes sense
    // for it to match between LARGE_NUMBER and 0
    // Another reason to not use regex
    printf("Max: %d // Min: %d\n", max, min);

    regular_expression->quantifier.type = RANGE;
    regular_expression->quantifier.max = max;
    regular_expression->quantifier.min = min;
    regular_expression->quantifier.modifier = GREEDY;

    return i;
}