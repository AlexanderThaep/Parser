#include <stdlib.h>
#include <Parsing.h>
#include <Error.h>

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
    struct TokenStack *t_stack = (struct TokenStack *)malloc(sizeof(struct TokenStack));
    struct Token *array = (struct Token *)malloc(sizeof(struct Token *) * size);

    if (t_stack == (struct TokenStack *) NULL || array == (struct Token *) NULL) 
    {
        return (struct TokenStack *) NULL;
    }

    t_stack->index = 0;
    t_stack->max = size;
    t_stack->toks = array;

    return t_stack;
}

struct Token *pushToken(struct TokenStack *stack, struct Token tok)
{
    if (stack->index >= stack->max)
        error("Max tokens exceeded (this probably should not be an issue)!", FATAL);

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

unsigned int processSingleQuotes(char *pointer)
{
    unsigned int i = 0;
    while (*pointer != '\'' || *pointer != '\0')
    {
        pointer++;
        i++;
    }
    return i;
}

unsigned int processSpecialCharacters(char *pointer)
{
    unsigned int i = 0;

}

struct TokenStack *parseInput(char *input)
{
    if (input == (char *)NULL || input[0] == '\0')
    {
        error("Cannot parse empty input", FATAL);
    }

    struct TokenStack *tokens = createTokenStack(DEFAULT_TOKEN_STACK_SIZE);

    if (tokens == (struct TokenStack *)NULL)
        error("Unable to allocate token buffer for input parsing", FATAL);

    unsigned int i = 0;
    char *token_addr = &input[i];
    char peek;

    struct Token token;
    token.id = -1;
    token.data = (char *)NULL;

    unsigned char curState = DEFAULT_STATE;
    unsigned char prevState = DEFAULT_STATE;
    unsigned char spawnToken = WORD;

    while (input[i] != '\0')
    {
        peek = input[i + 1];
        switch (input[i])
        {
        case '<':
        case '>':
        case '!':
        case '=':
        case '&':
        case '|':
            input[i] = '\0';
            curState = SPECIAL_STATE;
            spawnToken = WORD;
            break;
        case '{':
            input[i] = '\0';
            curState = CURLEY_STATE;
            spawnToken = LBRACE; 
            break;
        case '}':
            input[i] = '\0';
            curState = CURLEY_STATE;
            spawnToken = RBRACE;
            break;
        case ' ':
        case '\r':
        case '\v':
        case '\f':
        case '\t':
            input[i] = '\0';
            curState = WHITESPACE_STATE;
            break;
        default:
            curState = DEFAULT_STATE;
            break;
        }

        if (curState != prevState)
        {
            prevState = curState;

            if (curState)

            token = matchToken(token_addr);
            pushToken(tokens, token);
            token_addr = &input[i + 1];

            if (spawnToken != WORD) {
                token.id = spawnToken;
                token.data = (char *) NULL;
                pushToken(tokens, token);

                spawnToken = WORD;
            }
        }

        i++;
    }

    return tokens;
}