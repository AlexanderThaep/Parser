#include <stdio.h>
#include <stdlib.h>
#include <RegExp.h>
#include <Quantifier.h>

#define ASCII_9 58
#define ASCII_0 48
#define ASCII_COMMA 44

int feedQuantifier(RE* regular_expression, size_t i, char* re, size_t len) {
    int initial = i;
    int max = -1;
    int min = 0;

    int* place = &min;

    while (i < len) {
        if (re[i] <= ASCII_9 && re[i] >= ASCII_0) {
            (*place) = ((*place)*10) + (re[i] - 48);
        } else if (re[i] == ASCII_COMMA) {
            place = &max;
            max = 0;
        } else if (re[i] == '}') { i++; break; }
        i++;
    }

    if (max < 0) { max = min; }
    regular_expression->quantifier.type = MIN_MAX;
    regular_expression->quantifier.max = max;
    regular_expression->quantifier.min = min;
    
    return i - initial;
}