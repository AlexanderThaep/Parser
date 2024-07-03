#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Token.h>
#include <Error.h>

const static char *RESERVED_WORDS[] = {
    "if", "then", "else", "elif", "fi", "do",
    "done", "case", "esac", "until", "for"
};

// Shamelessly lifted from https://github.com/rui314/chibicc/blob/main/tokenize.c#L114
static inline bool startsWith(char *word, char *sequence)
{
    return strncmp(word, sequence, strlen(sequence)) == 0;
} 

// Please unroll me, compiler!
static inline char *inc(char *input)
{
    if (startsWith(input, "\\\n")) {
        return input + 2;
    }
    return input + 1;
}

static char escapedToLiteral(char *input)
{
    switch (*input) 
    { 
    case 'a': return '\a';
    case 'b': return '\b';
    case 't': return '\t';
    case 'n': return '\n';
    case 'v': return '\v';
    case 'f': return '\f';
    case 'r': return '\r';
    case '"': return '\"';
    case '\'': return '\'';
    case '\\': return '\\';
    default: return *input;
    }

    // Should not be reached
    return '\0';
}

static size_t appendToTokenWithBuffer(struct Token *tok, char c)
{
    size_t len = strlen(tok->begin);
    if (len >= tok->span)
    {
        char *temp = realloc(tok->begin, tok->span * 2);
        if (temp == NULL)
        {
            error("Cannot realloc token buffer", FATAL);
        }
        tok->begin = temp;
        tok->span *= 2;
    }
    *(tok->begin + len) = c;
    return len;
}

static struct Token *createTokenWithBuffer(unsigned int size)
{
    struct Token *tok = calloc(sizeof(char), sizeof(*tok));
    char *buffer = malloc(sizeof(char) * size);
    if (buffer == NULL || tok == NULL)
    {
        error("Could not allocate new token with buffer", FATAL);
    }
    tok->begin = buffer;
    tok->span = size;
    return tok;
}

static struct Token *processWord(char **input)
{
    struct Token *tok = createTokenWithBuffer(CHAR_BUFFER_SIZE);
    tok->id = WORD;

    bool onlyDigits = true;

    while (**input != '\0')
    {

        switch (**input)
        {
        case '<':
        case '>':
            if (onlyDigits)
            {
                tok->id = IO_NUMBER;
                return tok;
            }
        case '!':
        case '=':
        case '&':
        case '|':
        case '{':
        case '}':
        case ';':
        case '\n':
        case '\r':
        case '\v':
        case '\f':
        case '\t':
        case ' ':
            return tok;
        case '\\':
            (*input)++;
            char c = escapedToLiteral(*input);
            appendToTokenWithBuffer(tok, c);
            break;
        default:
            appendToTokenWithBuffer(tok, **input);
            break;
        }

        if (!(**input >= '0' && **input <= '9'))
        {
            onlyDigits = false;   
        }

        (*input)++;
    }

    return tok;
}

static struct Token *processName(char **input)
{
    struct Token *tok = createTokenWithBuffer(CHAR_BUFFER_SIZE);
    tok->id = NAME;
    
    while (**input == '_' 
    || (**input >= 'A' && **input <= 'Z')
    || (**input >= 'a' && **input <= 'z')
    || (**input >= '0' && **input <= '9'))
    {
        appendToTokenWithBuffer(tok, **input);
        (*input)++;
    }

    return tok;
}

static struct Token *processDoubleQuotes(char **input) {
    struct Token *tok = createTokenWithBuffer(CHAR_BUFFER_SIZE);
    tok->id = WORD;

    (*input)++;

    appendToTokenWithBuffer(tok, '\"');

    while (**input != '\"') {
        if (**input == '\0')
        {
            error("End reached before closing double quotes", FATAL);
        }
        switch (**input) {
        case '\\':
            (*input)++;
            if (**input == '\n') break;

            char c = escapedToLiteral(*input);
            appendToTokenWithBuffer(tok, c);
            break;
        case '$':
            // TODO
        case '`':
            // far future TODO
        default:
            appendToTokenWithBuffer(tok, **input);
            break;
        }
        (*input)++;
    }

    (*input)++;

    appendToTokenWithBuffer(tok, '\"');

    return tok;
}

static struct Token *processSingleQuotes(char **input) {
    struct Token *tok = malloc(sizeof(*tok));
    tok->begin = *input;
    tok->id = WORD;

    unsigned int i = 0;
    (*input)++;
    i++;

    while (**input != '\'') {
        if (**input == '\0')
        {
            error("End reached before closing single quote", FATAL);
        }
        (*input)++;
        i++;
    }
    (*input)++;

    tok->span = i;
    return tok;
}

struct Token *lex(char *input)
{
    if (input == NULL || *input == '\0')
    {
        error("Cannot parse empty input", FATAL);
    }

    struct Token head = {};
    struct Token *tok = &head;

    while (*input != '\0')
    {
        switch (*input)
        {
        case '#':
            // C taste
            for (*input; *input != '\n' && *input != '\0'; input++);
            continue;
        case '/':
            if (startsWith(input, "/*"))
            {
                char *closer = strstr(input + 2, "*/");
                if (closer == NULL) 
                {
                    error("Unclosed multiline comment somewhere", FATAL);
                }
                input = closer + 2;
                continue;
            }
            error("Why is there a random / somehwhere", FATAL);
        case '\'':
            tok->next = processSingleQuotes(&input);
            tok = tok->next;
            continue;
        case '\"':
            tok->next = processDoubleQuotes(&input);
            tok = tok->next;
            continue;
        case '$':
            if (startsWith(input, "$("))
            {
            // TODO    
            }
        case '`':
            // far future TODO
            input = inc(input);
            continue;
        case '\n':
            {
            struct Token *newln = createTokenWithBuffer(1);
            newln->begin = input;
            newln->span = 0;
            newln->id = NEWLINE;
            tok->next = newln;
            tok = tok->next;
            }
        case '\r':
        case '\v':
        case '\f':
        case '\t':
        case ' ':
            input = inc(input);
            continue;
        case '<':
        case '>':
        case '!':
        case '=':
        case '&':
        case '|':
        case '{':
        case '}':
        case '[':
        case ']':
        case ';':
            // operator
            input = inc(input);
            continue;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '_':
            tok->next = processName(&input);
            tok = tok->next;
            continue;
        default:
            tok->next = processWord(&input);
            tok = tok->next;
            break;
        }
    }

    return head.next;
}