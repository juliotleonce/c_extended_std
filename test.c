#include <assert.h>
#include <stdio.h>

#include "xarray.h"

void test_xarray();

int main() {
    test_xarray();
    return 0;
}

void test_xarray() {
    XArray *xarray = xarray_new(sizeof(int));
    for (int i = 0; i < 10; ++i) {
        int x = i * 10;
        xarray_push(xarray, &x);
        const int el = *(int*)xarray_get(xarray, i);
        printf("Element Number %d: %d\n", i+1, el);
        assert(el == x);
    }
    xarray_free(xarray);
}