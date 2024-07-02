#if !defined(PARSING)
#define PARSING

#define CHAR_BUFFER_SIZE 16

typedef enum
{
    WORD,
    ASSIGNMENT_WORD,
    NAME,
    NEWLINE,
    IO_NUMBER,
    AND_IF,
    OR_IF,
    DSEMI,
    DLESS,
    DGREAT,
    LESSAND,
    GREATAND,
    LESSGREAT,
    DLESSDASH,
    CLOBBER,
    If,
    Then,
    Else,
    Elif,
    Fi,
    Do,
    Done,
    Case,
    Esac,
    While,
    Until,
    For,
    Lbrace,
    Rbrace,
    Bang,
    In
} TokenType;

struct Token *lex(char *input);

// Recently just discovered that the Linux kernel style guide
// disallows typedef'ing structs and unions
// To an extent, I see the error of my ways
// On the other hand, >:(

struct __attribute__((__packed__)) Token
{
    struct Token *next;
    unsigned int span;
    char *begin;
    TokenType id;
};

#endif