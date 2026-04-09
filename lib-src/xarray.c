#include "../include/xarray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/xmemctl.h"

static void xarray_resize(XArray *xarray, unsigned new_length);

XArray *xarray_new(const size_t type_size) {
    XArray *xarray = xmem_alloc(sizeof(XArray));
    xarray->length = 0;
    xarray->type_size = type_size;
    xarray->c_tab = xmem_alloc(type_size);
    return xarray;
}

void xarray_push(XArray *xarray, const void *data) {
    xarray_resize(xarray, xarray->length + 1);
    memcpy(xarray->c_tab + (xarray->length - 1) * xarray->type_size, data, xarray->type_size);
}

void xarray_pop(XArray *xarray, void *output) {
    if (xarray->length == 0) {
        output = NULL;
    }
    const void *last_element = xarray->c_tab + (xarray->length - 1) * xarray->type_size;
    memcpy(output, last_element, xarray->type_size);
    xarray_resize(xarray, xarray->length - 1);
}

void *xarray_at(XArray *xarray, const unsigned index) {
    if (index >= xarray->length) {
        xarray_free(xarray);
        printf("Index out of bounds\n");
        exit(1);
    }
    return xarray->c_tab + index * xarray->type_size;
}

XArray *xarray_copy(const XArray *xarray) {
    XArray *copy = xarray_new(xarray->type_size);
    copy->length = xarray->length;
    copy->c_tab = xmem_alloc(xarray->length * xarray->type_size);
    memcpy(copy->c_tab, xarray->c_tab, xarray->length * xarray->type_size);
    return copy;
}

XArray *xarray_from_tab(const void *tab, const unsigned length, const size_t type_size) {
    XArray *xarray = xmem_alloc(sizeof(XArray));
    xarray->length = length;
    xarray->type_size = type_size;
    xarray->c_tab = xmem_alloc(length * type_size);
    memcpy(xarray->c_tab, tab, length * type_size);
    return xarray;
}

void xarray_free(XArray *xarray) {
    xmem_free(xarray->c_tab);
    xmem_free(xarray);
}

/**
 *
 * PRIVATE FUNCTION IMPLEMENTATION HERE
 */

void xarray_resize(XArray *xarray, const unsigned new_length) {
    void *temp = xmem_realloc(xarray->c_tab, new_length * xarray->type_size);
    if (temp == NULL) {
        printf("Can't push new element to array\n");
        xarray_free(xarray);
        exit(1);
    }
    xarray->c_tab = temp;
    xarray->length = new_length;
}