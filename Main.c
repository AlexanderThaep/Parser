#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>

int main(int argc, char** args) {
    char* regex = "(en)+";
    char* testex = "   senenentencesentence";

    RE** parseStack = parse(regex, 6);
    debug(parseStack, 0);

    boolState stateTest = test(parseStack, testex, 24);
    printf("Truth: %d\n", stateTest.match);
    printf("Index: %d\n", stateTest.consumed);
    match(parseStack, testex, 24);

    return 0;
}