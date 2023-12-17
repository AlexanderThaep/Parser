#include <stdlib.h>
#include <stdio.h>
#include <RegTypes.h>

int main(int argc, char** args) {
    char* regex = "(en).?";
    char* testex = "   sentencesentence";

    RE** parseStack = parse(regex, 7);
    debug(parseStack, 0);

    boolState stateTest = test(parseStack, testex, 20);
    printf("Truth: %d\n", stateTest.match);
    printf("Index: %d\n", stateTest.consumed);
    bulkTest(parseStack, testex, 20);

    return 0;
}