#include "../include/xmemctl.h"

#include <stdio.h>
#include <stdlib.h>

static XMemStack global_mem_stack = { NULL, 0 };

static void xmem_stack_add_ptr(void *ptr);
static void xmem_stack_update_ptr(const void *old_ptr, void *new_ptr);

void *xmem_alloc(const size_t size) {
    void *ptr = calloc(1, size);
    if (ptr == NULL) {
        printf("Can't allocate memory\n");
        exit(1);
    }

    xmem_stack_add_ptr(ptr);
    return ptr;
}

void xmem_free(const void *ptr) {
    XMemStackNode *node = global_mem_stack.top;
    while (node != NULL) {
        if (node->ptr == ptr) {
            free(node->ptr);
            node->ptr = NULL;
            return;
        }
        node = node->next;
    }
}

void *xmem_realloc(void *ptr, const size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        printf("Can't allocate memory\n");
        exit(1);
    }

    if (ptr != new_ptr) xmem_stack_update_ptr(ptr, new_ptr);
    return new_ptr;
}

void xmem_checkpoint() {
    uintptr_t *checkpoint_ptr = calloc(1, sizeof(uintptr_t));
    *checkpoint_ptr = global_mem_stack.current_checkpoint_ptr;
    global_mem_stack.current_checkpoint_ptr = (uintptr_t) checkpoint_ptr;
    xmem_stack_add_ptr(checkpoint_ptr);
}

void xmem_rollback() {
    const uintptr_t *checkpoint_ptr = (uintptr_t *) global_mem_stack.current_checkpoint_ptr;
    XMemStackNode *node = global_mem_stack.top;

    global_mem_stack.current_checkpoint_ptr = *checkpoint_ptr;

    while (node != NULL) {
        const uintptr_t ptr_addr = (uintptr_t) node->ptr;
        XMemStackNode *next_node = node->next;

        if (node->ptr != NULL) free(node->ptr);
        free(node);

        node = next_node;
        if (ptr_addr == (uintptr_t) checkpoint_ptr) break;
    }

    global_mem_stack.top = node;
}

void xmem_reset(void) {
    XMemStackNode *node = global_mem_stack.top;

    while (node != NULL) {
        XMemStackNode *next_node = node->next;

        if (node->ptr != NULL) free(node->ptr);
        free(node);

        node = next_node;
    }

    global_mem_stack.top = NULL;
    global_mem_stack.current_checkpoint_ptr = 0;
}


/**
 *
 * PRIVATE FUNCTION IMPLEMENTATION HERE
 */

void xmem_stack_add_ptr(void *ptr) {
    XMemStackNode *node = malloc(sizeof(XMemStackNode));
    node->ptr = ptr;
    node->next = global_mem_stack.top;
    global_mem_stack.top = node;
}

void xmem_stack_update_ptr(const void *old_ptr, void *new_ptr) {
    XMemStackNode *node = global_mem_stack.top;
    while (node != NULL) {
        if (node->ptr == old_ptr) {
            node->ptr = new_ptr;
            return;
        }
        node = node->next;
    }
}
