#include <stdio.h>
#include <stdlib.h>
#include <RegExp.h>

#define ASCII_FSLASH 92
#define ASCII_DASH 45

int feedLiterals(RE* regular_expression, size_t i, char* re, size_t len) {

    char last = 0;

    char table[255] = {0};

    while (i < len) {
        if (re[i] == ']') { return i; }

        if (i++ < len - 1) {
            switch (re[i]) {
                case ASCII_FSLASH:
                    table[re[i]] = 1;
                    last = re[i];
                    i++;
                    continue;
                case ASCII_DASH:
                    
                    i++;
                    continue;
            }
        }

        table[re[i]] = 1;
        last = re[i];

        i++;
    }
    
    return i;
}