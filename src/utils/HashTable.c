#include <stdio.h>
#include <stdint.h>

/*
First, str is dereferenced to get the character it points to. This character is 
assigned to c. str is then advanced to the next character. Finally (and this is the part you care about),
the value in c is checked against the null character, which is the test condition for the while loop. 
*/

// This hash works on 32-bit so make sure uint32_t

uint32_t djb2(unsigned char *str)
{
    uint32_t hash = 5381;
    uint32_t c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

uint32_t djb2_xor(unsigned char *str)
{
    uint32_t hash = 5381;
    uint32_t c;
    while (c = *str++)
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 ^ c */
    return hash;
}

unsigned long djb2_hash(char *str)
{
    uint32_t hash = 5381;
    unsigned char *s = (unsigned char *) str;
    uint32_t c = *s;
    while (c)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        s++;
        c = *s;
    }
    return (unsigned long) hash;
}

unsigned long djb2_xor_hash(char *str)
{
    uint32_t hash = 5381;
    unsigned char *s = (unsigned char *) str;
    uint32_t c = *s;
    while (c)
    {
        hash = ((hash << 5) + hash) ^ c; /* hash * 33 ^ c */
        s++;
        c = *s;
    }
    return (unsigned long) hash;
}