#include "ihct/ihct.h"
#include "src/utils/HashTable.c"

#if __INTELLISENSE__
#pragma diag_suppress 1094
#endif

IHCT_TEST(hash_test3)
{
    unsigned char test_string[] = "test";
    unsigned long hash = djb2_xor_hash(test_string);
    unsigned long correct = 0x7c7354f3;
    IHCT_ASSERT(correct = hash);
}

IHCT_TEST(hash_test2)
{
    unsigned long hash0 = djb2_hash("ab");
    unsigned long hash1 = djb2_hash("bA");
    IHCT_ASSERT(hash0 == hash1);
}

IHCT_TEST(hash_test1)
{
    unsigned long hash0 = djb2_xor("alexander");
    unsigned long hash1 = djb2_xor_hash("alexander");
    IHCT_ASSERT(hash0 == hash1);
}

IHCT_TEST(hash_test0)
{
    unsigned char test_string[] = "test";
    unsigned long hash = djb2(test_string);
    unsigned long correct = 0x7c9e6865;
    IHCT_ASSERT(correct == hash);
}

int main(int argc, char **argv) {
    return IHCT_RUN(argc, argv);
}