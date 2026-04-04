//
// Created by ajl420 on 3/26/26.
//

#ifndef EXTENDED_STD_XHASHSET_H
#define EXTENDED_STD_XHASHSET_H
#define INITIAL_CAPACITY 8
#define LOAD_FACTOR 0.75f
#define XHASHSET_NEW() xhashset_create()
#include <stdbool.h>
#include <stddef.h>

#include "xarray.h"

typedef unsigned (*XHashSetHashFn) (unsigned capacity, const void *element, size_t type_size);
typedef int (*XHashSetEqualFn) (const void *a, const void *b, size_t type_size);

typedef struct{
    void *value;
    unsigned psl;
    bool is_taken;
} XHashSetItem;

typedef struct{
    XHashSetItem *items;
    size_t type_size;
    unsigned capacity;
    unsigned items_account;
    XHashSetEqualFn equal_fn;
    XHashSetHashFn hash_fn;
} XHashSet;

XHashSet *xhashset_new(size_t type_size, XHashSetEqualFn equal_fn, XHashSetHashFn hash_fn);

void xhashset_add(XHashSet *xhashset, void *item_value);

bool xhashset_has(const XHashSet *xhashset, const void *item_value);

void xhashset_remove(XHashSet *xhashset, const void *item_value);

XHashSet *xhashset_union(const XHashSet *xhashset_a, const XHashSet *xhashset_b);

XHashSet *xhashset_intersection(const XHashSet *xhashset_a, const XHashSet *xhashset_b);

XHashSet *xhashset_difference(const XHashSet *xhashset_a, const XHashSet *xhashset_b);

XArray *xhashset_values(const XHashSet *xhashset);

void xhashset_clear(XHashSet *xhashset);

unsigned xhashset_default_hash_fn(unsigned capacity, const void *element, size_t type_size);

bool xhashset_default_equal_fn(const void *a, const void *b, size_t type_size);

void xhashset_free(XHashSet *xhashset);

#endif