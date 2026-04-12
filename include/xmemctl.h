//
// Created by ajl420 on 4/8/26.
//

#ifndef EXTENDED_STD_XMEMCTL_H
#define EXTENDED_STD_XMEMCTL_H
#include <stddef.h>
#include <stdint.h>
#define X_CONCAT_INTERNAL(a, b) a##b
#define X_CONCAT(a, b) X_CONCAT_INTERNAL(a, b)

#define XMEM_SCOPE \
    for (int X_CONCAT(_scope_i_, __LINE__) = (xmem_checkpoint(), 0); \
    X_CONCAT(_scope_i_, __LINE__) < 1; \
    X_CONCAT(_scope_i_, __LINE__)++, xmem_rollback())

typedef struct XMemStackNode {
    void *ptr;
    struct XMemStackNode *next;
} XMemStackNode;

typedef struct XMemStack {
    XMemStackNode *top;
    uintptr_t current_checkpoint_addr;
} XMemStack;

void *xmem_alloc(size_t size);

void xmem_free(const void *ptr);

void *xmem_realloc(void *ptr, size_t size);

void xmem_checkpoint();

void xmem_rollback();

void xmem_reset(void);

#endif //EXTENDED_STD_XMEMCTL_H