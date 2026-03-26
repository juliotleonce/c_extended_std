#include "../include/xhashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static float xhashmap_load_factor(const XHashMap *xhashmap);
static unsigned xhashmap_hash_key(const XHashMap *xhashmap, const char *key);
static void xhashmap_resize(XHashMap *xhashmap);
static void xhashmap_entry_copy(const XHashMapEntry *entry_to_copy, XHashMapEntry *entry_dest, size_t value_type_size);
static void xhashmap_entry_swap(XHashMapEntry *entry_a, XHashMapEntry *entry_b, size_t value_type_size);
static XHashMapEntry *xhashmap_entry_new(const char *key, const void *value, size_t value_type_size);
static void xhashmap_entry_clean_update(XHashMapEntry *entry, const char *key, const void *value, size_t value_type_size);
static void xhashmap_entry_update(XHashMapEntry *entry, const char *key, const void *value, size_t value_type_size);
static void xhashmap_entry_reset(XHashMapEntry *entry);

XHashMap *xhashmap_create_from_type_size(const size_t type_size) {
    XHashMap *xhashmap = calloc(1, sizeof(XHashMap));

    if (xhashmap == NULL) {
        printf("Can't allocate memory for XHashMap\n");
        exit(1);
    }

    xhashmap->entries = calloc(INITIAL_CAPACITY, sizeof(XHashMapEntry));
    xhashmap->capacity = INITIAL_CAPACITY;
    xhashmap->items_account = 0;
    xhashmap->type_size = type_size;

    return xhashmap;
}


void xhashmap_put(XHashMap *xhashmap, const char *key, const void *value) {
    if (xhashmap_load_factor(xhashmap) > LOAD_FACTOR) {
        xhashmap_resize(xhashmap);
    }

    unsigned index = xhashmap_hash_key(xhashmap, key);
    XHashMapEntry *entry_to_insert = xhashmap_entry_new(key, value, xhashmap->type_size);

    while (true) {
        XHashMapEntry *existing_entry = &xhashmap->entries[index];

        if (!existing_entry->is_taken) {
            xhashmap_entry_copy(entry_to_insert, existing_entry, xhashmap->type_size);
            xhashmap->items_account++;
            break;
        }

        if (strcmp(existing_entry->key, key) == 0) {
            free(entry_to_insert->value);
            entry_to_insert->value = calloc(1, xhashmap->type_size);
            memcpy(existing_entry->value, value, xhashmap->type_size);
            break;
        }

        if (entry_to_insert->psl > existing_entry->psl) {
            xhashmap_entry_swap(entry_to_insert, existing_entry, xhashmap->type_size);
        }

        entry_to_insert->psl++;
        index = (index + 1) % xhashmap->capacity;
    }

    free(entry_to_insert);
}

XArray *xhashmap_keys(const XHashMap* xhashmap){
    XArray *xarray = xarray_create_from_type_size(xhashmap->type_size);
    for (unsigned i = 0; i < xhashmap->capacity; ++i) {
        if (xhashmap->entries[i].is_taken) {
            xarray_push(xarray, xhashmap->entries[i].key);
        }
    }
    return xarray;
}

XArray *xhashmap_value(const XHashMap* xhashmap){
    XArray *xarray = xarray_create_from_type_size(xhashmap->type_size);
    for (unsigned i = 0; i < xhashmap->capacity; ++i) {
        if (xhashmap->entries[i].is_taken) {
            xarray_push(xarray, xhashmap->entries[i].value);
        }
    }
    return xarray;
}

void xhashmap_remove(XHashMap *xhashmap, const char *key) {
    unsigned index = xhashmap_hash_key(xhashmap, key);
    unsigned psl = 0;

    while (true) {
        XHashMapEntry *entry = &xhashmap->entries[index];

        if (!entry->is_taken) break;
        if (entry->psl < psl) break;
        if (strcmp(entry->key, key) == 0) {
            xhashmap_entry_reset(entry);
            xhashmap->items_account--;
        }

        psl++;
        index = (index + 1) % xhashmap->capacity;
    }
}

void *xhashmap_get(const XHashMap *xhashmap, const char *key) {
    unsigned index = xhashmap_hash_key(xhashmap, key);
    unsigned psl = 0;

    while (true) {
        const XHashMapEntry *entry = &xhashmap->entries[index];

        if (!entry->is_taken) return NULL;
        if (entry->psl < psl) return NULL;
        if (strcmp(entry->key, key) == 0) return entry->value;

        psl++;
        index = (index + 1) % xhashmap->capacity;
    }
}

void xhashmap_free(XHashMap *xhashmap) {
    for (unsigned i = 0; i < xhashmap->capacity; ++i) {
        if (xhashmap->entries[i].is_taken) {
            xhashmap_entry_reset(&xhashmap->entries[i]);
        }
    }
    free(xhashmap->entries);
    free(xhashmap);
}

/**
 * PRIVATE FUNCTION IMPLEMENTATION GOES BELOW
 *
 */



static float xhashmap_load_factor(const XHashMap *xhashmap) {
    return (float)xhashmap->items_account / (float)xhashmap->capacity;
}

static unsigned xhashmap_hash_key(const XHashMap *xhashmap, const char *key) {
    unsigned int h = 5381;
    while (*key)
        h = (h << 5) + h ^ (unsigned char)*key++;
    return h % xhashmap->capacity;
}

static void xhashmap_resize(XHashMap *xhashmap) {
    const unsigned old_capacity = xhashmap->capacity;
    XHashMapEntry *old_entries = xhashmap->entries;

    xhashmap->capacity *= 2;
    xhashmap->items_account = 0;
    xhashmap->entries = calloc(xhashmap->capacity, sizeof(XHashMapEntry));

    for (unsigned i = 0; i < old_capacity; ++i) {
        if (old_entries[i].is_taken) {
            xhashmap_put(xhashmap, old_entries[i].key, old_entries[i].value);
        }
    }

    free(old_entries);
}

static void xhashmap_entry_copy(const XHashMapEntry *entry_to_copy, XHashMapEntry *entry_dest, const size_t value_type_size) {
    const char *copy_key = entry_to_copy->key;
    const void *copy_value = entry_to_copy->value;
    xhashmap_entry_clean_update(entry_dest, copy_key, copy_value, value_type_size);
    entry_dest->psl = entry_to_copy->psl;
}

static void xhashmap_entry_swap(XHashMapEntry *entry_a, XHashMapEntry *entry_b, const size_t value_type_size) {
    char temp_value[value_type_size];
    XHashMapEntry *temp = xhashmap_entry_new("", &temp_value, value_type_size);

    xhashmap_entry_copy(entry_a, temp, value_type_size);
    xhashmap_entry_copy(entry_b, entry_a, value_type_size);
    xhashmap_entry_copy(temp, entry_b, value_type_size);

    xhashmap_entry_reset(temp);
    free(temp);
}

static XHashMapEntry *xhashmap_entry_new(const char *key, const void *value, const size_t value_type_size) {
    XHashMapEntry *entry = calloc(1, sizeof(XHashMapEntry));
    if (entry == NULL) {
        printf("Can't allocate memory for XHashMapEntry\n");
        exit(1);
    }
    xhashmap_entry_update(entry, key, value, value_type_size);
    return entry;
}

static void xhashmap_entry_update(XHashMapEntry *entry, const char *key, const void *value,const size_t value_type_size) {
    entry->key = calloc(1, strlen(key) + 1);
    entry->value = calloc(1, value_type_size);
    entry->is_taken = true;

    memcpy(entry->key, key, strlen(key) + 1);
    memcpy(entry->value, value, value_type_size);
}

static void xhashmap_entry_clean_update(XHashMapEntry *entry, const char *key, const void *value, const size_t value_type_size) {
    const unsigned psl = entry->psl;
    xhashmap_entry_reset(entry);
    xhashmap_entry_update(entry, key, value, value_type_size);
    entry->psl = psl;
}

static void xhashmap_entry_reset(XHashMapEntry *entry) {
    entry->psl = 0;
    entry->is_taken = false;
    if (entry->key != NULL) {
        free(entry->key);
        entry->key = NULL;
    }
    if (entry->value != NULL) {
        free(entry->value);
        entry->value = NULL;
    }
}





