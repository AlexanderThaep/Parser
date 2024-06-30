#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>
#include <Error.h>

struct RE *createRegularExpression(int type, int quantifier, char data)
{
    struct RE *regular_expression = (struct RE *)malloc(sizeof(struct RE));
    if (regular_expression == (struct RE *)NULL)
    {
        return (struct RE *)NULL;
    }

    regular_expression->type = type;
    regular_expression->data = data;
    regular_expression->matches = 0;

    regular_expression->quantifier.type = quantifier;
    regular_expression->quantifier.max = 1;
    regular_expression->quantifier.min = 1;
    regular_expression->quantifier.modifier = GREEDY;

    regular_expression->child_stack = (struct RE **)NULL;
    regular_expression->table = (char *)NULL;

    return regular_expression;
}

struct RE **createParseStack(int size)
{
    struct RE **parse_stack = (struct RE **)malloc(sizeof(struct RE *) * size);
    if (parse_stack == (struct RE **)NULL || size < 1)
    {
        return (struct RE **)NULL;
    }

    struct RE *first_regexp = createRegularExpression(NONE, NONE, size);
    parse_stack[0] = first_regexp;

    return parse_stack;
}

struct RE *pushStack(struct RE **stack, struct RE *regular_expression)
{
    if (regular_expression == (struct RE *)NULL)
    {
        return (struct RE *)NULL;
    }

    stack[0]->type++;
    // I'm not sure I'd find many to approve of this, but
    // I used the first element of the array as a way to keep track of it
    // i.e max elements, current index, works fine. In hindsight,
    // I should have wrapped it in a struct
    int index = stack[0]->type;

    if (index >= stack[0]->data)
    {
        error("Stack overflow!", FATAL);
    }

    stack[index] = regular_expression;

    return regular_expression;
}

struct RE *peekStack(struct RE **stack)
{
    int index = stack[0]->type;
    if (index < 0 || stack[index] == (struct RE *)NULL)
    {
        return (struct RE *)NULL;
    }

    if (stack[index]->type == PORTAL)
    {
        struct RE **address = stack[index]->child_stack;
        free(stack[index]);
        stack[0]->type--;
        return peekStack(address);
    }

    return stack[index];
}

struct RE *popStack(struct RE **stack)
{
    int index = stack[0]->type;
    if (index < 1)
    {
        return (struct RE *)NULL;
    }

    struct RE *regular_expression = peekStack(stack);
    stack[0]->type--;
    return regular_expression;
}

void debug(struct RE **parseStack, int level)
{
    if (parseStack[0] != (struct RE *)NULL)
    {
        for (int i = 0; i <= parseStack[0]->type; i++)
        {
            printf("LEVEL: %d\n", level);
            printf("--TYPE: %d\n", parseStack[i]->type);
            printf("--QUANTIFIER: %d\n", parseStack[i]->quantifier.type);
            printf("----MODIFIER: %d\n", parseStack[i]->quantifier.modifier);
            printf("--DATA: %d\n", parseStack[i]->data);
            printf("--CHILDSTACK: %p\n", parseStack[i]->child_stack);
            if (parseStack[i]->child_stack != (struct RE **)NULL)
            {
                debug(parseStack[i]->child_stack, level + 1);
            }
        }
    }
}

struct RE **parse(char *re, size_t len)
{

    struct RE **parse_stack = createParseStack(DEFAULT_STACK_SIZE);
    if (parse_stack == (struct RE **)NULL)
    {
        return (struct RE **)NULL;
    }

    struct RE **child_stack = createParseStack(DEFAULT_STACK_SIZE);
    parse_stack[0]->child_stack = child_stack;

    size_t i = 0;
    while (i < len)
    {
        struct RE *regular_expression = (struct RE *)NULL;
        struct RE *lastRE;
        switch (re[i])
        {
        case '.':
            regular_expression = createRegularExpression(WILDCARD, EXACTLY_ONE, re[i]);
            pushStack(peekStack(parse_stack)->child_stack, regular_expression);
            break;
        case '?':
            lastRE = peekStack(peekStack(parse_stack)->child_stack);
            if (lastRE == (struct RE *)NULL || lastRE->quantifier.type == NONE)
            {
                error("? struct Quantifier has to follow something", FATAL);
            }
            lastRE->quantifier.modifier = LAZY;
            if (lastRE->quantifier.type == EXACTLY_ONE)
            {
                lastRE->quantifier.type = RANGE;
                lastRE->quantifier.modifier = GREEDY;
                lastRE->quantifier.max = 1;
                lastRE->quantifier.min = 0;
            }
            break;
        case '+':
            lastRE = peekStack(peekStack(parse_stack)->child_stack);
            if (lastRE == (struct RE *)NULL || lastRE->quantifier.type == NONE)
            {
                error("+ struct Quantifier has to follow something", FATAL);
            }
            lastRE->quantifier.modifier = POSSESSIVE;
            if (lastRE->quantifier.type == EXACTLY_ONE)
            {
                lastRE->quantifier.type = RANGE;
                lastRE->quantifier.modifier = GREEDY;
                lastRE->quantifier.max = INT16_MAX;
                lastRE->quantifier.min = 1;
            }
            break;
        case '*':
            lastRE = peekStack(peekStack(parse_stack)->child_stack);
            if (lastRE == (struct RE *)NULL || lastRE->quantifier.type == NONE)
            {
                error("* struct Quantifier has to follow an exactly one", FATAL);
            }
            lastRE->quantifier.modifier = GREEDY;
            if (lastRE->quantifier.type == EXACTLY_ONE)
            {
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
            if (regular_expression == (struct RE *)NULL || regular_expression->type == OR_GROUP)
            {
                error("Too many ) parentheses!", FATAL);
            }

            pushStack(peekStack(parse_stack)->child_stack, regular_expression);
            break;
        case '{':
            lastRE = peekStack(peekStack(parse_stack)->child_stack);
            if (lastRE == (struct RE *)NULL || lastRE->quantifier.type != EXACTLY_ONE)
            {
                error("Bracketed quantifier has to follow an exactly one", FATAL);
            }

            i = feedQuantifier(lastRE, i, re, len);
            break;
        case '[':
            regular_expression = createRegularExpression(LITERAL_GROUP, EXACTLY_ONE, re[i]);
            pushStack(peekStack(parse_stack)->child_stack, regular_expression);

            i = feedLiteral(regular_expression, i + 1, re, len);
            break;
        case '\\':
            if (++i < len)
            {
                switch (re[i])
                {
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
            if (lastRE == (struct RE *)NULL || lastRE->quantifier.type == NONE)
            {
                error("| has to follow something", FATAL);
            }

            struct RE *portal = createRegularExpression(PORTAL, NONE, NONE);
            portal->child_stack = peekStack(parse_stack)->child_stack;

            if (lastRE->type != OR_GROUP)
            {
                regular_expression = createRegularExpression(OR_GROUP, EXACTLY_ONE, re[i]);
                regular_expression->child_stack = createParseStack(DEFAULT_STACK_SIZE / 2);

                pushStack(peekStack(parse_stack)->child_stack, regular_expression);
                pushStack(regular_expression->child_stack, lastRE);
            }
            else
            {
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

    if (parse_stack[0]->type > 0)
    {
        int index = parse_stack[0]->type;
        switch (parse_stack[index]->type)
        {
        case GROUP:
            error("Incomplete parentheses", FATAL);
            break;
        case OR_GROUP:
            error("| must have both sides", FATAL);
            break;
        }
    }

    struct RE **return_stack = parse_stack[0]->child_stack;

    free(parse_stack[0]);
    free(parse_stack);

    return return_stack;
}