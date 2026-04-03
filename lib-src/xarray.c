#include "../include/xarray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

XArray *xarray_new(const size_t type_size) {
    XArray *xarray = malloc(sizeof(XArray));
    xarray->length = 0;
    xarray->type_size = type_size;
    xarray->data = malloc(type_size);
    return xarray;
}

void xarray_push(XArray *xarray, const void *data) {
    xarray->length++;
    void *temp = realloc(xarray->data, xarray->length * xarray->type_size);
    if (temp == NULL) {
        printf("Can't push new element to array\n");
        xarray_free(xarray);
        exit(1);
    }
    xarray->data = temp;
    memcpy(xarray->data + (xarray->length - 1) * xarray->type_size, data, xarray->type_size);
}

void *xarray_get(XArray *xarray, const unsigned index) {
    if (index >= xarray->length) {
        xarray_free(xarray);
        printf("Index out of bounds\n");
        exit(1);
    }
    return xarray->data + index * xarray->type_size;
}

void xarray_free(XArray *xarray) {
    free(xarray->data);
    free(xarray);
}
