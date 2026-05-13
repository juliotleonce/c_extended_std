/**
 * @file xmemctl.h
 * @brief Memory control and automatic cleanup utilities.
 */

#ifndef EXTENDED_STD_XMEMCTL_H
#define EXTENDED_STD_XMEMCTL_H
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Internal macro for concatenating tokens.
 */
#define X_CONCAT_INTERNAL(a, b) a##b

/**
 * @brief Macro for concatenating tokens.
 */
#define X_CONCAT(a, b) X_CONCAT_INTERNAL(a, b)

/**
 * @brief Macro to define a memory scope.
 * All memory allocated with xmem_alloc within this scope will be automatically
 * freed when the scope is exited.
 */
#define XMEM_SCOPE \
    for (int X_CONCAT(_scope_i_, __LINE__) = (xmem_checkpoint(), 0); \
    X_CONCAT(_scope_i_, __LINE__) < 1; \
    X_CONCAT(_scope_i_, __LINE__)++, xmem_rollback())

/**
 * @struct XMemStackNode
 * @brief Internal structure for tracking memory allocations.
 */
typedef struct XMemStackNode {
    void *ptr;                      /**< Pointer to the allocated memory. */
    struct XMemStackNode *next;     /**< Pointer to the next node in the stack. */
} XMemStackNode;

/**
 * @struct XMemStack
 * @brief Stack structure to manage memory checkpoints.
 */
typedef struct XMemStack {
    XMemStackNode *top;             /**< Top of the allocation stack. */
    uintptr_t current_checkpoint_addr; /**< Current checkpoint address. */
} XMemStack;

/**
 * @brief Allocates memory that is tracked by the memory controller.
 * @param size Size of the memory block to allocate.
 * @return Pointer to the allocated memory.
 */
void *xmem_alloc(size_t size);

/**
 * @brief Manually frees memory allocated with xmem_alloc.
 * @param ptr Pointer to the memory block to free.
 */
void xmem_free(const void *ptr);

/**
 * @brief Reallocates memory tracked by the memory controller.
 * @param ptr Pointer to the memory block to reallocate.
 * @param size New size of the memory block.
 * @return Pointer to the reallocated memory.
 */
void *xmem_realloc(void *ptr, size_t size);

/**
 * @brief Sets a memory checkpoint.
 */
void xmem_checkpoint();

/**
 * @brief Rolls back to the last memory checkpoint, freeing all memory allocated since then.
 */
void xmem_rollback();

/**
 * @brief Resets the memory controller, freeing all tracked memory and clearing checkpoints.
 */
void xmem_reset(void);

#endif //EXTENDED_STD_XMEMCTL_H