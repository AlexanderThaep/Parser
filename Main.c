#include <stdlib.h>
#include <stdio.h>
#include <RegTypes.h>

int main(int argc, char** args) {
    char* regex = "a(b.)?cd";
    char* testex = "ab=cd";

    RE** parseStack = parse(regex, 9);
    debug(parseStack, 0);

    boolState stateTest = test(parseStack, testex, 6);
    printf("Truth: %d\n", stateTest.match);
    printf("Index: %d\n", stateTest.consumed);

    return 0;
}