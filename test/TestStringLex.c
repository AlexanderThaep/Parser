#include "ihct/ihct.h"
#include "src/core/Error.c"
#include "src/interface/Lexer.c"

// Courtesy of https://github.com/microsoft/vscode-cpptools/issues/4503https://github.com/microsoft/vscode-cpptools/issues/4503
// Some unresolved issue with intellisense since 2019
#if __INTELLISENSE__
#pragma diag_suppress 1094
#endif

IHCT_TEST(string_test4)
{
    char string[] = "\" \\n \\\\ \\\" \\\n still \"";
    char *test_string = string;
    char valid_string[] = "\" \n \\ \"  still \"";
    TokenType valid_token_type = WORD;
    struct Token *token = processDoubleQuotes(&test_string);
    IHCT_ASSERT(valid_token_type == token->id);
    IHCT_ASSERT(strncmp(valid_string, token->begin, token->span) == 0);
    IHCT_ASSERT(strlen(valid_string) == strlen(token->begin));
}

IHCT_TEST(string_test3)
{
    char string[] = "\"this is a double quoted string\"";
    char *test_string = string;
    TokenType valid_token_type = WORD;
    struct Token *token = processDoubleQuotes(&test_string);
    IHCT_ASSERT(valid_token_type == token->id);
    IHCT_ASSERT(strncmp(string, token->begin, token->span) == 0);
    IHCT_ASSERT(strlen(string) == strlen(token->begin));
}

IHCT_TEST(string_test2)
{
    char string[] = "\'\'\'";
    char *test_string = string;
    TokenType valid_token_type = WORD;
    struct Token *token = processSingleQuotes(&test_string);
    IHCT_ASSERT(valid_token_type == token->id);
    IHCT_ASSERT(strncmp(string, token->begin, token->span) == 0);
    IHCT_ASSERT(strlen(string) != token->span + 1);
}

IHCT_TEST(string_test1)
{
    char string[] = "\'string \\\n980:{>\'";
    char *test_string = string;
    TokenType valid_token_type = WORD;
    struct Token *token = processSingleQuotes(&test_string);
    IHCT_ASSERT(valid_token_type == token->id);
    IHCT_ASSERT(strncmp(string, token->begin, token->span) == 0);
    IHCT_ASSERT(strlen(string) == token->span + 1);
}

IHCT_TEST(string_test0)
{
    char string[] = "\'this is a string\'";
    char *test_string = string;
    TokenType valid_token_type = WORD;
    struct Token *token = processSingleQuotes(&test_string);
    IHCT_ASSERT(valid_token_type == token->id);
    IHCT_ASSERT(strncmp(string, token->begin, token->span) == 0);
    IHCT_ASSERT(strlen(string) == token->span + 1);
}

int main(int argc, char **argv) {
    return IHCT_RUN(argc, argv);
}