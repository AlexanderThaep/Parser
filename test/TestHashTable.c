#include "ihct/ihct.h"
#include "src/utils/HashTable.c"

#if __INTELLISENSE__
#pragma diag_suppress 1094
#endif

static inline void addTableHelper(
    struct bch_table *table, const char *key, const char *value)
{
    const char *test_key = key;
    const char *test_value = strndup(value, strlen(value));
    struct bch_llist_slot *item = insert_bch_table(table, test_key, test_value);
    if (item) printf("Key: %s || Value: %s\n", item->key, item->data);
}

IHCT_TEST(hash_test3)
{
    struct bch_table *table = make_bch_table(64, 4, 2, &fnv_1_hash, &fnv_1a_hash);
    addTableHelper(table, "army", "ranger");
    addTableHelper(table, "army", "ranger");
    addTableHelper(table, "nap", "ster");
    addTableHelper(table, "berlin", "versailles");
    addTableHelper(table, "karl", "marx");
    addTableHelper(table, "8=D", "^69");
    addTableHelper(table, "420", "herb");
    addTableHelper(table, "green", "fn");
    addTableHelper(table, "alksdadawdad", "adniaw820");
    addTableHelper(table, "alexander", "t");
    printf("Used: %lu\n", table->info.used);
    struct bch_llist_slot *s = find_bch_table(table, "army");
    printf("Found: %p\n", s);
    if (s) printf("Found data: %s\n", s->data);
    remove_bch_table(table, "army");
    printf("Used: %lu\n", table->info.used);
    destroy_bch_table(table);
    IHCT_PASS();
}

IHCT_TEST(hash_test2)
{
    unsigned long hash0 = djb2_hash("ab");
    unsigned long hash1 = djb2_hash("bA");
    const char test_string[] = "test";
    unsigned long hash = djb2(test_string);
    unsigned long correct = 0x7c9e6865;
    IHCT_ASSERT(correct == hash);
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
    unsigned long hash = fnv_1_hash("geronimo");
    unsigned long correct = 0x04224043;
    IHCT_ASSERT(correct == hash);
}

int main(int argc, char **argv) {
    return IHCT_RUN(argc, argv);
}