#ifndef EXTENDED_STD_XHASHMAP_H
#define EXTENDED_STD_XHASHMAP_H
#define INITIAL_CAPACITY 8
#define LOAD_FACTOR 0.75f
#define XHASHMAP_NEW(ItemsValueType) xhashmap_new(sizeof(ItemsValueType))
#include <stdbool.h>
#include <stddef.h>
#include "xarray.h"

typedef struct {
    char *key;
    void *value;
    bool is_taken;
    unsigned psl;
} XHashMapEntry;

typedef struct {
    XHashMapEntry *entries;
    unsigned capacity;
    unsigned items_account;
    size_t type_size;
} XHashMap;

XHashMap *xhashmap_new(size_t type_size);

void xhashmap_put(XHashMap *xhashmap, const char *key, const void *value);

XArray *xhashmap_keys(const XHashMap *xhashmap);

XArray *xhashmap_values(const XHashMap *xhashmap);

void xhashmap_remove(XHashMap *xhashmap, const char *key);

void *xhashmap_get(const XHashMap *xhashmap, const char *key);

void xhashmap_free(const XHashMap *xhashmap);

char **keys_to_array(const XHashMap *xhashmap, unsigned *out_count);

#endif //EXTENDED_STD_XHASHMAP_H