#include <stdlib.h>
#include <stdio.h>

#include <Error.h>

const static char *ERRORS[] =
{
    "Undefined",
    "Generic",
    "Fatal"
};

void error(char *message, unsigned int type)
{
    fprintf(stderr, "[ERROR] %s :::: [TYPE] %s\n", message, ERRORS[type]);
    exit(EXIT_FAILURE);
}