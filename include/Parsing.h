#if !defined(PARSING)
#define PARSING

#define WORD 0
#define ASSIGNMENT_WORD 1
#define NAME 2
#define NEWLINE 3
#define IO_NUMBER 4
#define AND_IF 5
#define OR_IF 6
#define DLESS 7
#define DGREAT 8
#define IN 9
#define LBRACE 10
#define RBRACE 11
#define BANG 12

#define If 13
#define Then 14
#define Else 15
#define Elif 16
#define Fi 17
#define Do 18
#define Done 19
#define Case 20
#define Esac 21
#define Until 22
#define For 23

#define DEFAULT_TOKEN_STACK_SIZE 64

struct Token *parseInput(char *buffer);

// Recently just discovered that the Linux kernel style guide
// disallows typedef'ing structs and unions
// To an extent, I see the error of my ways
// On the other hand, >:(

struct TokenStack {
    unsigned int max;
    unsigned int index;
    struct Token * toks;
};

struct __attribute__((__packed__)) Token
{
    // unsigned int id;
    char *data;
    unsigned char id;
};

const char *RESERVED_WORDS[] = {
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

#endif