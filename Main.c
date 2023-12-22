#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <RegExp.h>

int main(int argc, char** args) {
    char* regex = ".*b";
    char* testex = "aaabcd";

    RE** parseStack = parse(regex, strlen(regex));
    debug(parseStack, 0);

    boolState stateTest = test(parseStack, testex, strlen(testex));
    printf("Truth: %d\n", stateTest.match);
    printf("Index: %d\n", stateTest.consumed);

    return 0;
}