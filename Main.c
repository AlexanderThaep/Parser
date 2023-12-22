#include <stdlib.h>
#include <stdio.h>
#include <RegExp.h>

int main(int argc, char** args) {
    char* regex = "[^a-n]+";
    char* testex = "   sene5eent9ence675sentence8756";

    RE** parseStack = parse(regex, 8);
    debug(parseStack, 0);

    boolState stateTest = test(parseStack, testex, 33);
    printf("Truth: %d\n", stateTest.match);
    printf("Index: %d\n", stateTest.consumed);
    match(parseStack, testex, 33);

    return 0;
}