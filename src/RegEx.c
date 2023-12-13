#include <stdlib.h>
#include <stdio.h>
#include <RegTypes.h>

RE* createRegularExpression(int type, int quantifier) {
    RE* regular_expression = (RE*) malloc(sizeof(RE));
    if (regular_expression == (RE*) NULL) { return (RE*) NULL; }

    regular_expression->type = type;
    regular_expression->quantifier = quantifier;
    regular_expression->child_stack = (RE**) NULL;

    return regular_expression;
}

RE** createParseStack(int size) {
    RE** parse_stack = (RE**) malloc(sizeof(RE*) * size);
    if (parse_stack == (RE**) NULL || size < 1) { return (RE**) NULL; }

    RE* first_regexp = createRegularExpression(NONE, size);
    parse_stack[0] = first_regexp;

    return parse_stack;
}

RE* pushStack(RE** stack, RE* regular_expression) {
    if (regular_expression == (RE*) NULL) { return (RE*) NULL; }

    stack[0]->type++;

    int index = stack[0]->type;
    char* err_msg = "Stack overflow!";
    if (index >= stack[0]->quantifier) { error(err_msg, 2); }

    stack[index] = regular_expression;

    return regular_expression;
}

RE* peekStack(RE** stack) {
    int index = stack[0]->type;
    if (index < 0) { return (RE*) NULL; }
    
    return stack[index];
}

RE* popStack(RE** stack) {
    int index = stack[0]->type;
    if (index < 1) { return (RE*) NULL; }

    RE* regular_expression = peekStack(stack);
    stack[0]->type--;
    return regular_expression;
}

void debug(RE* regular_expression, int level) {
    printf("LEVEL: %d\n", level);
    printf("--TYPE: %d\n", regular_expression->type);
    printf("--QUANTIFIER: %d\n", regular_expression->quantifier);
    if (regular_expression->child_stack != (RE**) NULL
    && regular_expression->child_stack[0] != (RE*) NULL) {
        for (int i = 0; i <= regular_expression->child_stack[0]->type; i++) {
            if (regular_expression->child_stack[i] != (RE*) NULL) {
                debug(regular_expression->child_stack[i], level + 1);
            } 
        }
    }
}

RE* parse(char* re, size_t len) {

    RE** parse_stack = createParseStack(64);
    if (parse_stack == (RE**) NULL) { return (RE*) NULL; }

    RE** child_stack = createParseStack(64);
    parse_stack[0]->child_stack = child_stack;

    size_t i = 0;
    while (i < len) {
        RE* regular_expression;
        switch (re[i]) {
            case '.':
                regular_expression = createRegularExpression(WILDCARD, EXACTLY_ONE);
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
            case '?':
                regular_expression = createRegularExpression(WILDCARD, ZERO_OR_ONE);
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
            case '+':
                regular_expression = createRegularExpression(WILDCARD, ONE_OR_MORE);
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
            case '*':
                regular_expression = createRegularExpression(WILDCARD, ZERO_OR_MORE);
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
            case '(':
                regular_expression = createRegularExpression(GROUP, EXACTLY_ONE);
                regular_expression->child_stack = createParseStack(64);

                pushStack(parse_stack, regular_expression);
                break;
            case ')':
                regular_expression = popStack(parse_stack);
                if (regular_expression == (RE*) NULL) {
                    error("Too many ) parentheses!", 2);
                }
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
            case '\\':
                break;
        }
        i++;
    }
    
    return parse_stack[0];
}