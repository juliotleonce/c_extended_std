#ifndef EXTENDED_STD_XHASHMAP_H
#define EXTENDED_STD_XHASHMAP_H
#define INITIAL_CAPACITY 8
#define LOAD_FACTOR 0.75f
#define XHASHMAP_NEW(ItemsValueType) xhashmap_create_from_type_size(sizeof(ItemsValueType))
#include <stdbool.h>
#include <stddef.h>

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

XHashMap *xhashmap_create_from_type_size(size_t type_size);

void xhashmap_put(XHashMap *xhashmap, const char *key, const void *value);

void *xhashmap_get(const XHashMap *xhashmap, const char *key);

void xhashmap_free(XHashMap *xhashmap);

#endif //EXTENDED_STD_XHASHMAP_H