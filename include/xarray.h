//
// Created by ajl420 on 3/24/26.
//

#ifndef EXTENDED_STD_XARRAY_H
#define EXTENDED_STD_XARRAY_H
#include <stdlib.h>

#define XARRAY_NEW(ElementType) xarray_new(sizeof(ElementType))

typedef struct XArray {
    unsigned length;
    size_t type_size;
    void *c_tab;
} XArray;

XArray *xarray_new(size_t type_size);

void xarray_push(XArray *xarray, const void *data);

void xarray_pop(XArray *xarray, void *output);

void *xarray_get(XArray *xarray, unsigned index);

XArray *xarray_copy(const XArray *xarray);

XArray *xarray_from_tab(const void *tab, unsigned length, size_t type_size);

void xarray_free(XArray *xarray);

#endif