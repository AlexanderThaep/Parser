#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <RegExp.h>
#include <Interface.h>

// int main(int argc, char** args) {
//     char* regex = "(er.e){1,}";
//     // char* regex = ".[a-z]*";
//     // char* testex = "However, this implementation seeks to allow for the minimum that any user familiar with RegEx would appreciate. There are few special features and even fewer guarantees of efficiency or function. Yet, it's a little novelty project that I hope to incorporate into a larger Language Parser and, eventually, compiler.";
//     char* testex = "a limbless cold-blooded vertebrate animal with gills and fins and living wholly in water: the sea is thick with fish.";

//     struct RE** parseStack = parse(regex, strlen(regex));
//     debug(parseStack, 0);

//     struct BoolState stateTest = test(parseStack, testex, strlen(testex));
//     printf("Truth: %d\n", stateTest.match);
//     printf("Consumed: %d\n", stateTest.consumed);
//     match(parseStack, testex, strlen(testex));

//     return 0;
// }

// we shall invoke the interface to provide an easy manner of testing and utilizing the engine
// all of this will be handled here in Main
int main(int argc, char **argv)
{
    if (argc > 1)
    {


        return 0;
    }

    initInterface();

    if (shell_is_interactive)
    {
        int exit_shell = 0;
        int echo_back = 1;
        int actual_buffer_size = INTERFACE_BUFFER_SIZE;

        char *input = (char *)malloc(sizeof(char) * actual_buffer_size);
        // memset(input, '\0', actual_buffer_size);

        welcomeScreen();

        while (!exit_shell)
        {
            prompt();
            processTerminal(input, actual_buffer_size);

            if (echo_back)
            {
                // printf("%lu\n", sizeof(struct Token));
                printf("%s\n", input);
            }
        }
        
        free(input);
    }

    return 0;
}