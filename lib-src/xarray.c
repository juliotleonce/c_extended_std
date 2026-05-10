#include "../include/xarray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/xmemctl.h"

static void xarray_resize(XArray *xarray, size_t new_capacity);

XArray *xarray_new(const size_t type_size) {
    XArray *xarray = xmem_alloc(sizeof(XArray));
    xarray->length = 0;
    xarray->type_size = type_size;
    xarray->capacity = XARRAY_INITIAL_CAPACITY;
    xarray->c_tab = xmem_alloc(type_size * XARRAY_INITIAL_CAPACITY);
    return xarray;
}

void xarray_push(XArray *xarray, const void *data) {
    if (xarray->capacity < xarray->length + 1) {
        const size_t new_capacity = xarray->capacity + (xarray->capacity >> 1);
        xarray_resize(xarray, new_capacity);
    }
    xarray->length++;
    memcpy(xarray->c_tab + (xarray->length - 1) * xarray->type_size, data, xarray->type_size);
}

void xarray_pop(XArray *xarray, void *output) {
    if (xarray->length == 0) output = NULL;

    const void *last_element = xarray->c_tab + (xarray->length - 1) * xarray->type_size;
    memcpy(output, last_element, xarray->type_size);
    xarray->length--;

    if (xarray->length == xarray->capacity >> 1) {
        const size_t new_capacity = xarray->capacity - (xarray->capacity >> 1);
        xarray_resize(xarray, new_capacity);
    }
}

void *xarray_at(const XArray *xarray, const unsigned index) {
    if (index >= xarray->length) return NULL;
    return xarray->c_tab + index * xarray->type_size;
}

XArray *xarray_copy(const XArray *xarray) {
    XArray *copy = xarray_new(xarray->type_size);
    copy->length = xarray->length;
    xarray_resize(copy, xarray->capacity);
    memcpy(copy->c_tab, xarray->c_tab, xarray->length * xarray->type_size);
    return copy;
}

XArray *xarray_from_tab(const void *tab, const unsigned length, const size_t type_size) {
    XArray *xarray = xmem_alloc(sizeof(XArray));

    xarray->length = length;
    xarray->type_size = type_size;
    xarray->capacity = length + (length >> 1);
    xarray->c_tab = xmem_alloc(xarray->capacity * type_size);
    memcpy(xarray->c_tab, tab, length * type_size);

    return xarray;
}

XArray *xarray_slice(const XArray *xarray, const unsigned start, const unsigned end) {
    XArray *slice = xarray_new(xarray->type_size);
    slice->length = end - start;
    slice->c_tab = xmem_alloc(slice->length * xarray->type_size);
    memcpy(slice->c_tab, xarray->c_tab + start * xarray->type_size, slice->length * xarray->type_size);
    return slice;
}

void xarray_free(const XArray *xarray) {
    xmem_free(xarray->c_tab);
    xmem_free(xarray);
}

/**
 *
 * PRIVATE FUNCTION IMPLEMENTATION HERE
 */

void xarray_resize(XArray *xarray, const size_t new_capacity) {
    xarray->capacity = new_capacity;
    xarray->c_tab = xmem_realloc(xarray->c_tab, xarray->capacity * xarray->type_size);
}