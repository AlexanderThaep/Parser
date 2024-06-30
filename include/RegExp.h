#if !defined(REGEXP)
#define REGEXP
#define NONE 0x000

#define WILDCARD 0x001
#define LITERAL 0x002
#define GROUP 0x003
#define LITERAL_GROUP 0x004
#define OR_GROUP 0x005
#define PORTAL 0x006

#define EXACTLY_ONE 0x001
#define ZERO_OR_ONE 0x002
#define ZERO_OR_MORE 0x003
#define RANGE 0x004

#define GREEDY 0x000
#define POSSESSIVE 0x001
#define LAZY 0x002

#define ASCII_9 58
#define ASCII_0 48
#define ASCII_COMMA 44
#define ASCII_FSLASH 92
#define ASCII_DASH 45
#define ASCII_CARET 94

#define DEFAULT_STACK_SIZE 64
#define MATCHES_ARRAY 512
#define ASCII_TABLE 256

struct Quantifier
{
    unsigned short type;
    unsigned short modifier;
    unsigned short min;
    unsigned short max;
};

// Here, it does not matter if type or data is char and matches is short
// It will still be aligned by 8 and make 32 bytes
// So, might as well short and int it up
struct RE
{
    unsigned short type;
    unsigned short data;
    unsigned int matches;
    struct Quantifier quantifier;
    struct RE **child_stack;
    char *table;
};

// So, structs addressed (what the struct address refers to) by its first element
// and are aligned according to the largest member; here, we read 12 bytes since 4 by 3
// is 12
struct BoolState
{
    unsigned int consumed;
    unsigned int match;
    unsigned int end;
};

struct RE **parse(char *re, size_t len);
struct BoolState test(struct RE **stack, char *string, size_t len);
struct BoolState *match(struct RE **stack, char *string, size_t len);

void debug(struct RE **parseStack, int level);

int feedQuantifier(struct RE *regular_expression, size_t i, char *re, size_t len);
int feedLiteral(struct RE *regular_expression, size_t i, char *re, size_t len);

#endif