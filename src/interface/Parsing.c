#include <stdlib.h>
#include <Parsing.h>

const static char *RESERVED_WORDS[] = {
    "if",
    "then",
    "else",
    "elif",
    "fi",
    "do",
    "done",
    "case",
    "esac",
    "until",
    "for"
};

struct TokenStack *createTokenStack(unsigned int size)
{
    struct TokenStack *t_stack = (struct TokenStack *)malloc(sizeof(struct TokenStack) * size);
    struct Token *array = (struct Token *)malloc(sizeof(struct Token *) * size);

    t_stack->index = 0;
    t_stack->max = size;
    t_stack->toks = array;

    return t_stack;
}

struct Token *pushToken(struct TokenStack *stack, struct Token tok)
{
    if (stack->index >= stack->max)
        return (struct Token *)NULL;
    // I'll deal with handling this error later...

    stack->toks[stack->index].data = tok.data;
    stack->toks[stack->index].id = tok.id;

    stack->index++;

    return &stack->toks[stack->index];
}

struct Token matchToken(char *str)
{
    struct Token tok;
    tok.id = -1;
    tok.data = str;

    return tok;
}

unsigned int skipSingleQuotes(char *pointer)
{
    unsigned int i = 0;
    while (*pointer != '\'' || *pointer != '\0')
    {
        pointer++;
        i++;
    }
    return i;
}

struct TokenStack *parseInput(char *input)
{
    if (input == (char *)NULL || input[0] == '\0')
    {
        exit(EXIT_FAILURE);
    }

    struct TokenStack *tokens = (struct TokenStack *)malloc(sizeof(struct TokenStack));

    if (tokens == (struct Token *)NULL)
        goto ENDME;

    unsigned int i = 0;
    char c = input[i];
    char *token_addr = &input[i];
    char peek;

    struct Token token;
    token.id = -1;
    token.data = (char *)NULL;

    while (c != '\0')
    {
        peek = input[i + 1];
        switch (c)
        {
        case '\'':
            input[i] = '\0';
            token = matchToken(token_addr);
            pushToken(tokens, token);
            token_addr = &input[i + 1];

            i += skipSingleQuotes(&input[i + 1]);
            token.id = WORD;
            token.data = token_addr;

            input[i] = '\0';
            // token = matchToken(token_addr);
            pushToken(tokens, token);
            token_addr = &input[i + 1];
            break;
        case '<':
        case '>':
        case '!':
        case '=':
            switch (peek)
            {
            case ' ':
                break;
            }
        case ' ':
        case '\r':
        case '\v':
        case '\f':
        case '\t':
            input[i] = '\0';
            token = matchToken(token_addr);
            pushToken(tokens, token);
            token_addr = &input[i + 1];
            break;
        default:
            break;
        }
        i++;
    }

ENDME:
    return tokens;
}