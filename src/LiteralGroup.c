#include <stdio.h>
#include <stdlib.h>
#include <RegExp.h>

#define ASCII_FSLASH 92
#define ASCII_DASH 45

void activateLiteral(char* table, char literal, char last, int* addr_active_ranging) {
    if ((*addr_active_ranging) == 1) {
        for (int i = last; i < literal; i++) {
            table[i] = 1;
        }
        (*addr_active_ranging) = 0;
        return;
    }
    table[literal] = 1;
}

int feedLiterals(RE* regular_expression, size_t i, char* re, size_t len) {

    char* table = (char*) calloc(sizeof(char) * 255, 0);
    if (table == (char*) NULL) { return 0; }

    char last = NULL;
    int active_ranging = 0;

    while (i < len) {
        if (re[i] == ']') { break; }

        if (i + 1 < len - 1) {
            switch (re[i]) {
                case ASCII_FSLASH:
                    activateLiteral(table, re[++i], last, &active_ranging);
                    last = re[i];
                    i++;
                    continue;
                case ASCII_DASH:
                    active_ranging = 1;
                    i++;
                    continue;
                default:
                    break;
            }
        }

        activateLiteral(table, re[i], last, &active_ranging);
        last = re[i];
        i++;
    }

    regular_expression->table = table;
    
    return i;
}