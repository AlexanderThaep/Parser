#if !defined(REGEXP)
#define REGEXP
#define NONE               0x000

#define WILDCARD           0x001
#define LITERAL            0x002
#define GROUP              0x003
#define LITERAL_GROUP      0x004

#define EXACTLY_ONE        0x001
#define ZERO_OR_ONE        0x002
#define ZERO_OR_MORE       0x003
#define RANGE              0x004

#define GREEDY             0x000
#define POSSESSIVE         0x001
#define LAZY               0x002

#define ASCII_9               58
#define ASCII_0               48
#define ASCII_COMMA           44
#define ASCII_FSLASH          92
#define ASCII_DASH            45
#define ASCII_CARET           94

#define DEFAULT_STACK_SIZE    64
#define MATCHES_ARRAY        512
#define ASCII_TABLE          256

typedef struct Quantifier {
    unsigned short type;
    unsigned short modifier;
    unsigned short min;
    unsigned short max;
} Quantifier;

typedef struct RE {
    unsigned char type;
    unsigned char data;
    unsigned short matches;
    struct Quantifier quantifier;
    struct RE** child_stack;
    char* table;
} RE;

typedef struct BoolState {
    unsigned int consumed;
    unsigned int match;
    unsigned int end;
} BoolState;

int error(char* message, int type);

RE** parse(char* re, size_t len);
BoolState test(RE** stack, char* string, size_t len);
BoolState* match(RE** stack, char* string, size_t len);

void debug(RE** parseStack, int level);

int feedQuantifier(RE* regular_expression, size_t i, char* re, size_t len);
int feedLiteral(RE* regular_expression, size_t i, char* re, size_t len);

#endif