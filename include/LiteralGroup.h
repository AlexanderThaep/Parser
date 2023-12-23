#include <stdlib.h>
#include <RegExp.h>

#define ASCII_FSLASH    92
#define ASCII_DASH      45
#define ASCII_CARET     94

#define ASCII_TABLE     256

int feedLiterals(RE* regular_expression, size_t i, char* re, size_t len);