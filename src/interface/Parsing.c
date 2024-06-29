#include <stdlib.h>
#include <Parsing.h>

struct Token pushToken(struct Token *toks, struct Token tok) 
{

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
    while (*pointer != '\'' || *pointer != '\0') {
        pointer++;
        i++;
    }
    return i;
}

struct Token *parseInput(char *input) 
{
    if (input == (char *) NULL || input[0] == '\0') {
        exit(EXIT_FAILURE);
    }

    struct Token * tokens = (struct Token *) malloc(sizeof(struct Token) * DEFAULT_TOKEN_STACK_SIZE);

    if (tokens == (struct Token *) NULL) goto ENDME;

    unsigned int i = 0;
    char c = input[i];
    char *token_addr = &input[i];
    char peek;

    struct Token token;
    token.id = -1;
    token.data = (char *) NULL;

    while (c != '\0') {
        peek = input[i + 1];
        switch (c) {
            case '\'':
                input[i] = '\0';
                token_addr = &input[i + 1];           
                i += skipSingleQuotes(&input[i + 1]);

            case '<':
            case '>':
            case '!':
            case '=':
            case ' ':
            case '\r':
            case '\v':
            case '\f':
            case '\t':
                input[i] = '\0';
                token = matchToken(token_addr);
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