//
// Created by ajl420 on 4/8/26.
//

#ifndef EXTENDED_STD_XMEMCTL_H
#define EXTENDED_STD_XMEMCTL_H
#include <stddef.h>
#include <stdint.h>

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