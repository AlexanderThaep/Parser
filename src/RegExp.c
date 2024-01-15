#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>

RE* createRegularExpression(int type, int quantifier, char data) {
    RE* regular_expression = (RE*) malloc(sizeof(RE));
    if (regular_expression == (RE*) NULL) { return (RE*) NULL; }

    regular_expression->type = type;
    regular_expression->data = data;
    regular_expression->matches = 0;

    regular_expression->quantifier.type = quantifier;
    regular_expression->quantifier.max = 1;
    regular_expression->quantifier.min = 1;
    regular_expression->quantifier.modifier = GREEDY;

    regular_expression->child_stack = (RE**) NULL;
    regular_expression->table = (char*) NULL;

    return regular_expression;
}

RE** createParseStack(int size) {
    RE** parse_stack = (RE**) malloc(sizeof(RE*) * size);
    if (parse_stack == (RE**) NULL || size < 1) { return (RE**) NULL; }

    RE* first_regexp = createRegularExpression(NONE, NONE, size);
    parse_stack[0] = first_regexp;

    return parse_stack;
}

RE* pushStack(RE** stack, RE* regular_expression) {
    if (regular_expression == (RE*) NULL) { return (RE*) NULL; }

    stack[0]->type++;

    int index = stack[0]->type;

    if (index >= stack[0]->data) { error("Stack overflow!", 2); }

    stack[index] = regular_expression;

    return regular_expression;
}

RE* peekStack(RE** stack) {
    int index = stack[0]->type;
    if (index < 0 || stack[index] == (RE*) NULL) { return (RE*) NULL; }

    if (stack[index]->type == PORTAL) {
        RE** address = stack[index]->child_stack;
        free(stack[index]);
        stack[0]->type--;
        return peekStack(address);
    }
    
    return stack[index];
}

RE* popStack(RE** stack) {
    int index = stack[0]->type;
    if (index < 1) { return (RE*) NULL; }

    RE* regular_expression = peekStack(stack);
    stack[0]->type--;
    return regular_expression;
}

void debug(RE** parseStack, int level) {
    if (parseStack[0] != (RE*) NULL) {
        for (int i = 0; i <= parseStack[0]->type; i++) {
            printf("LEVEL: %d\n", level);
            printf("--TYPE: %d\n", parseStack[i]->type);
            printf("--QUANTIFIER: %d\n", parseStack[i]->quantifier.type);
            printf("----MODIFIER: %d\n", parseStack[i]->quantifier.modifier);
            printf("--DATA: %d\n", parseStack[i]->data);
            printf("--CHILDSTACK: %p\n", parseStack[i]->child_stack);
            if (parseStack[i]->child_stack != (RE**) NULL) {
                debug(parseStack[i]->child_stack, level + 1);
            } 
        }
    }
}

RE** parse(char* re, size_t len) {

    RE** parse_stack = createParseStack(DEFAULT_STACK_SIZE);
    if (parse_stack == (RE**) NULL) { return (RE**) NULL; }

    RE** child_stack = createParseStack(DEFAULT_STACK_SIZE);
    parse_stack[0]->child_stack = child_stack;

    size_t i = 0;
    while (i < len) {
        RE* regular_expression = (RE*) NULL;
        RE* lastRE;
        switch (re[i]) {
            case '.':
                regular_expression = createRegularExpression(WILDCARD, EXACTLY_ONE, re[i]);
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
            case '?':
                lastRE = peekStack(peekStack(parse_stack)->child_stack);
                if (lastRE == (RE*) NULL || lastRE->quantifier.type == NONE) {
                    error("? Quantifier has to follow something", 2);
                }
                lastRE->quantifier.modifier = LAZY;
                if (lastRE->quantifier.type == EXACTLY_ONE) {
                    lastRE->quantifier.type = RANGE;
                    lastRE->quantifier.modifier = GREEDY;
                    lastRE->quantifier.max = 1;
                    lastRE->quantifier.min = 0;
                }
                break;
            case '+':
                lastRE = peekStack(peekStack(parse_stack)->child_stack);
                if (lastRE == (RE*) NULL || lastRE->quantifier.type == NONE) {
                    error("+ Quantifier has to follow something", 2);
                }
                lastRE->quantifier.modifier = POSSESSIVE;
                if (lastRE->quantifier.type == EXACTLY_ONE) {
                    lastRE->quantifier.type = RANGE;
                    lastRE->quantifier.modifier = GREEDY;
                    lastRE->quantifier.max = INT16_MAX;
                    lastRE->quantifier.min = 1;
                }
                break;
            case '*':
                lastRE = peekStack(peekStack(parse_stack)->child_stack);
                if (lastRE == (RE*) NULL || lastRE->quantifier.type == NONE) {
                    error("* Quantifier has to follow an exactly one", 2);
                }
                lastRE->quantifier.modifier = GREEDY;
                if (lastRE->quantifier.type == EXACTLY_ONE) {
                    lastRE->quantifier.type = RANGE;
                    lastRE->quantifier.modifier = GREEDY;
                    lastRE->quantifier.max = INT16_MAX;
                    lastRE->quantifier.min = 0;
                }
                break;
            case '(':
                regular_expression = createRegularExpression(GROUP, EXACTLY_ONE, re[i]);
                regular_expression->child_stack = createParseStack(DEFAULT_STACK_SIZE);

                pushStack(parse_stack, regular_expression);
                break;
            case ')':
                regular_expression = popStack(parse_stack);
                if (regular_expression == (RE*) NULL || regular_expression->type == OR_GROUP) {
                    error("Too many ) parentheses!", 2);
                }

                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
            case '{':
                lastRE = peekStack(peekStack(parse_stack)->child_stack);
                if (lastRE == (RE*) NULL || lastRE->quantifier.type != EXACTLY_ONE) {
                    error("Bracketed quantifier has to follow an exactly one", 2);
                }

                i = feedQuantifier(lastRE, i, re, len);
                break;
            case '[':
                regular_expression = createRegularExpression(LITERAL_GROUP, EXACTLY_ONE, re[i]);
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);

                i = feedLiteral(regular_expression, i + 1, re, len);
                break;
            case '\\':
                if (++i < len) {
                    switch (re[i]) {
                        case 'S':
                            regular_expression = createRegularExpression(GROUP, EXACTLY_ONE, re[i]);
                            regular_expression->child_stack = parse("[^ \n\t\v\f\r]", 9);
                            break;
                        case 'D':
                            regular_expression = createRegularExpression(GROUP, EXACTLY_ONE, re[i]);
                            regular_expression->child_stack = parse("[^0-9]", 6);
                            break;
                        case 'W':
                            regular_expression = createRegularExpression(GROUP, EXACTLY_ONE, re[i]);
                            regular_expression->child_stack = parse("[^a-zA-Z0-9]", 12);
                            break;
                        case 's':
                            regular_expression = createRegularExpression(GROUP, EXACTLY_ONE, re[i]);
                            regular_expression->child_stack = parse("[ \n\t\v\f\r]", 9);
                            break;
                        case 'd':
                            regular_expression = createRegularExpression(GROUP, EXACTLY_ONE, re[i]);
                            regular_expression->child_stack = parse("[0-9]", 6);
                            break;
                        case 'w':
                            regular_expression = createRegularExpression(GROUP, EXACTLY_ONE, re[i]);
                            regular_expression->child_stack = parse("[a-zA-Z0-9]", 12);
                            break;
                        case '\0':
                            break;
                        default:
                            regular_expression = createRegularExpression(LITERAL, EXACTLY_ONE, re[i]);
                            break;
                    }
                    pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                }
                break;
            case '|':
                lastRE = popStack(peekStack(parse_stack)->child_stack);
                if (lastRE == (RE*) NULL || lastRE->quantifier.type == NONE) {
                    error("| has to follow something", 2);
                }

                RE* portal = createRegularExpression(PORTAL, NONE, NONE);
                portal->child_stack = peekStack(parse_stack)->child_stack;

                if (lastRE->type != OR_GROUP) {
                    regular_expression = createRegularExpression(OR_GROUP, EXACTLY_ONE, re[i]);
                    regular_expression->child_stack = createParseStack(DEFAULT_STACK_SIZE / 2);

                    pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                    pushStack(regular_expression->child_stack, lastRE);
                } else {
                    pushStack(peekStack(parse_stack)->child_stack, lastRE);
                }

                pushStack(parse_stack, portal);

                break;
            case '\0':
                break;
            default:
                regular_expression = createRegularExpression(LITERAL, EXACTLY_ONE, re[i]);
                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                break;
        }
        i++;
    }
    
    if (parse_stack[0]->type > 0) {
        int index = parse_stack[0]->type;
        switch (parse_stack[index]->type) {
            case GROUP:
                error("Incomplete parentheses", 2);
                break;
            case OR_GROUP:
                error("| must have both sides", 2);
                break;
        }
    }

    RE** return_stack = parse_stack[0]->child_stack;

    free(parse_stack[0]);
    free(parse_stack);

    return return_stack;
}