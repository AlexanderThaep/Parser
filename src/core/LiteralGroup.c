#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <RegExp.h>

void activateLiteral(char *table, char literal, char last, int *addr_active_ranging)
{
    int index = (int)literal;
    if ((*addr_active_ranging) == 1)
    {
        for (int i = last + 1; i <= literal; i++)
        {
            table[i] = 1 - table[i];
        }
        (*addr_active_ranging) = 0;
        return;
    }
    table[index] = 1 - table[index];
}

int feedLiteral(struct RE *regular_expression, size_t i, char *re, size_t len)
{

    char *table = (char *)calloc(ASCII_TABLE, sizeof(char));
    if (table == (char *)NULL)
    {
        return 0;
    }

    if (re[i] == '^')
    {
        memset(table, 1, sizeof(char) * ASCII_TABLE);
    }

    char last = '\0';
    int active_ranging = 0;

    while (i < len)
    {
        if (re[i] == ']')
        {
            break;
        }

        if (i + 1 < len - 1)
        {
            switch (re[i])
            {
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

    table[0] = 0;
    regular_expression->table = table;

    return i;
}