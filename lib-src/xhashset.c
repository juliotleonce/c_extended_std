#include "../include/xhashset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static float xhashset_load_factor(const XHashSet *xhashset);
static void xhashset_resize(XHashSet *xhashset);
static void xhashset_item_reset(XHashSetItem *item);
static bool xhashset_is_mergeable(const XHashSet *xhashset_a, const XHashSet *xhashset_b);

XHashSet *xhashset_new(const size_t type_size, const XHashSetEqualFn equal_fn, const XHashSetHashFn hash_fn){
    XHashSet *xhashset = calloc(1, sizeof(XHashSet));

    if (xhashset == NULL){
        printf("Can't allocate memory for XHashSet\n");
        exit(1);
    }

    xhashset->items = calloc(INITIAL_CAPACITY, sizeof(XHashSetItem));
    xhashset->capacity = INITIAL_CAPACITY;
    xhashset->items_account = 0;
    xhashset->type_size = type_size;
    xhashset->equal_fn = equal_fn;
    xhashset->hash_fn = hash_fn;

    return xhashset;
}

void xhashset_add(XHashSet *xhashset, const void *item_value){
    if (xhashset_load_factor(xhashset) > LOAD_FACTOR){
        xhashset_resize(xhashset);
    }

    unsigned index = xhashset->hash_fn(xhashset->capacity, item_value, xhashset->type_size);
    void *current_value = malloc(xhashset->type_size);
    unsigned psl = 0;

    memcpy(current_value, item_value, xhashset->type_size);

    while (true){
        XHashSetItem *existing_item = &xhashset->items[index];

        if (!existing_item->is_taken){
            existing_item->value = current_value;
            existing_item->is_taken = true;
            existing_item->psl = psl;
            xhashset->items_account++;
            break;
        }

        if (xhashset->equal_fn(existing_item->value, item_value, xhashset->type_size)) {
            free(current_value);
            break;
        }

        if (existing_item->psl < psl) {
            void *tmp_value  = existing_item->value;
            const unsigned tmp_psl = existing_item->psl;

            existing_item->value = current_value;
            existing_item->psl = psl;

            current_value = tmp_value;
            psl = tmp_psl;
        }

        psl++;
        index = (index + 1) % xhashset->capacity;
    }
}

bool xhashset_has(const XHashSet *xhashset, const void *item_value) {
    unsigned index = xhashset->hash_fn(xhashset->capacity, item_value, xhashset->type_size);
    unsigned psl = 0;

    while (true) {
        const XHashSetItem *item = &xhashset->items[index];

        if (!item->is_taken) return false;
        if (item->psl < psl) return false;
        if (xhashset->equal_fn(item->value, item_value, xhashset->type_size)) return true;

        psl++;
        index = (index + 1) % xhashset->capacity;
    }
}

void xhashset_remove(XHashSet *xhashset, const void *item_value) {
    unsigned index = xhashset->hash_fn(xhashset->capacity, item_value, xhashset->type_size);
    unsigned psl = 0;

    while (true) {
        XHashSetItem *item = &xhashset->items[index];

        if (!item->is_taken) return;
        if (item->psl < psl) return;
        if (xhashset->equal_fn(item->value, item_value, xhashset->type_size)) {
            xhashset_item_reset(item);
            xhashset->items_account--;

            while (true) {
                const unsigned neighbour_index = (index + 1) % xhashset->capacity;
                XHashSetItem *neighbour = &xhashset->items[neighbour_index];

                if (!neighbour->is_taken || neighbour->psl == 0) break;

                item->value = calloc(xhashset->type_size, sizeof(char));
                memcpy(item->value, neighbour->value, xhashset->type_size);
                item->psl = neighbour->psl - 1;
                item->is_taken = true;

                xhashset_item_reset(neighbour);

                index = neighbour_index;
                item = &xhashset->items[index];
            }
            return;
        }

        psl++;
        index = (index + 1) % xhashset->capacity;
    }
}

XHashSet *xhashset_union(const XHashSet *xhashset_a, const XHashSet *xhashset_b) {
    if (xhashset_is_mergeable(xhashset_a, xhashset_b) == false) {
        printf("Can't union sets with different types\n");
        exit(1);
    }

    XHashSet *union_set = xhashset_new(
        xhashset_a->type_size,
        xhashset_a->equal_fn,
        xhashset_a->hash_fn
    );

    for (unsigned i = 0; i < xhashset_a->capacity; ++i) {
        if (xhashset_a->items[i].is_taken) xhashset_add(union_set, xhashset_a->items[i].value);
    }

    for (unsigned i = 0; i < xhashset_b->capacity; ++i) {
        if (xhashset_b->items[i].is_taken) xhashset_add(union_set, xhashset_b->items[i].value);
    }

    return union_set;
}

XHashSet *xhashset_intersection(const XHashSet *xhashset_a, const XHashSet *xhashset_b) {
    if (xhashset_is_mergeable(xhashset_a, xhashset_b) == false) {
        printf("Can't intersect sets with different types\n");
        exit(1);
    }

    XHashSet *intersection_set = xhashset_new(
        xhashset_a->type_size,
        xhashset_a->equal_fn,
        xhashset_a->hash_fn
    );

    for (unsigned i = 0; i < xhashset_a->capacity; ++i) {
        if (!xhashset_a->items[i].is_taken) continue;
        void *item_value = xhashset_a->items[i].value;
        if (!xhashset_has(xhashset_b, item_value)) continue;
        xhashset_add(intersection_set, item_value);
    }

    return intersection_set;
}

XHashSet *xhashset_difference(const XHashSet *xhashset_a, const XHashSet *xhashset_b) {
    if (xhashset_is_mergeable(xhashset_a, xhashset_b) == false) {
        printf("Can't difference sets with different types\n");
        exit(1);
    }

    XHashSet *difference_set = xhashset_new(
        xhashset_a->type_size,
        xhashset_a->equal_fn,
        xhashset_a->hash_fn
    );

    for (unsigned i = 0; i < xhashset_a->capacity; ++i) {
        if (!xhashset_a->items[i].is_taken) continue;
        void *item_value = xhashset_a->items[i].value;
        if (!xhashset_has(xhashset_b, item_value)) {
            xhashset_add(difference_set, item_value);
        }
    }

    return difference_set;
}

XArray *xhashset_values(const XHashSet *xhashset) {
    XArray *xarray = xarray_new(xhashset->type_size);

    for (unsigned i = 0; i < xhashset->capacity; ++i) {
        if (!xhashset->items[i].is_taken) continue;
        xarray_push(xarray, xhashset->items[i].value);
    }

    return xarray;
}

void xhashset_clear(XHashSet *xhashset) {
    for (unsigned i = 0; i < xhashset->capacity; ++i) {
        if (xhashset->items[i].is_taken) {
            xhashset_item_reset(&xhashset->items[i]);
        }
    }

    free(xhashset->items);
    xhashset->items = calloc(INITIAL_CAPACITY, sizeof(XHashSetItem));
    xhashset->capacity = INITIAL_CAPACITY;
    xhashset->items_account = 0;
}

unsigned xhashset_default_hash_fn(const unsigned capacity, const void *element, const size_t type_size) {
    unsigned int h = 5381;
    unsigned i = 0;
    const char *element_char = (char*) element;
    while (i < type_size) {
        h = (h << 5) + h ^ (unsigned char)*(element_char+i);
        i++;
    }
    return h % capacity;
}

bool xhashset_default_equal_fn(const void *a, const void *b, const size_t type_size) {
    return memcmp(a, b, type_size) == 0;
}

void xhashset_free(XHashSet *xhashset){
    for (unsigned i = 0; i < xhashset->capacity; ++i) {
        if (xhashset->items[i].is_taken) {
            free(xhashset->items[i].value);
        }
    }
    free(xhashset->items);
    free(xhashset);
}

/**
 * PRIVATE FUNCTION IMPLEMENTATION GOES BELOW
 *
 */

float xhashset_load_factor(const XHashSet *xhashset) {
    return (float)xhashset->items_account / (float)xhashset->capacity;
}

void xhashset_resize(XHashSet *xhashset) {
    const unsigned old_capacity = xhashset->capacity;
    XHashSetItem *old_items = xhashset->items;

    xhashset->capacity *= 2;
    xhashset->items_account = 0;
    xhashset->items = calloc(xhashset->capacity, sizeof(XHashSetItem));

    for (unsigned i = 0; i < old_capacity; ++i) {
        if (old_items[i].is_taken) {
            xhashset_add(xhashset, old_items[i].value);
        }
    }

    free(old_items);
}

void xhashset_item_reset(XHashSetItem *item) {
    item->psl = 0;
    item->is_taken = false;
    if (item->value != NULL) {
        free(item->value);
        item->value = NULL;
    }
}

bool xhashset_is_mergeable(const XHashSet *xhashset_a, const XHashSet *xhashset_b) {
    if (xhashset_a->type_size != xhashset_b->type_size) return false;
    if (xhashset_a->equal_fn != xhashset_b->equal_fn) return false;
    if (xhashset_a->hash_fn != xhashset_b->hash_fn) return false;
    return true;
}

