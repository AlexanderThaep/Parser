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
#define MIN_MAX            0x004

#define DEFAULT_STACK_SIZE    64

struct Quantifier {
    unsigned int type;
    unsigned int min:8;
    unsigned int max:8;
};

typedef struct RE {
    unsigned int type:8;
    unsigned int data:8;
    char* table;
    struct RE** child_stack;
    struct Quantifier quantifier;
} RE;

typedef struct boolState {
    unsigned int consumed;
    unsigned int match;
    unsigned int end;
} boolState;

int error(char* message, int type);

RE** parse(char* re, size_t len);
boolState test(RE** stack, char* string, size_t len);
boolState* match(RE** stack, char* string, size_t len);

void debug(RE** parseStack, int level);

#endif