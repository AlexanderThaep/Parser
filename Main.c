#include <stdlib.h>
#include <stdio.h>
#include <RegTypes.h>

int main(int argc, char** args) {
    char* regex = "((*)?.)";
    RE* parsed = parse(regex, 8);
    debug(parsed, 0);
    return 0;
}