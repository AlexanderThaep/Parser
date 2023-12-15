#define NONE               0x000

#define WILDCARD           0x001
#define LITERAL            0x002
#define GROUP              0x003

#define EXACTLY_ONE        0x001
#define ZERO_OR_ONE        0x002
#define ZERO_OR_MORE       0x003

#define DEFAULT_STACK_SIZE    64

typedef struct RE {
    int type;
    int quantifier;
    int data;
    struct RE** child_stack;
} RE;

typedef struct boolState {
    int consumed;
    int match;
} boolState;

int error(char* message, int type);

RE** parse(char* re, size_t len);
boolState test(RE** stack, char* string, size_t len);

void debug(RE** parseStack, int level);