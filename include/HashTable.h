#include <stdlib.h>
#include <stdint.h>

#if !defined(HASHTABLE)
#define HASHTABLE

#define DEFAULT_HTABLE_BUCKETS 64
#define DEFAULT_BUCKET_SLOTS 4

#define HASH_BITS uint32_t

struct bch_llist_slot
{
    void *data;
    const char *key;
    struct bch_llist_slot *next;
};

struct bch_llist_bucket
{
    size_t slots_used;
    struct bch_llist_slot *slot;
};

struct bch_buckets
{
    struct bch_llist_bucket *bucket;
    HASH_BITS (*hash_f)(const char *key);
};

struct bch_table_info
{
    size_t used;
    size_t buckets;
    size_t bucket_slots;
}; 

struct bch_table
{
    size_t table_count;
    struct bch_buckets *tables;
    struct bch_table_info info;
};

#endif