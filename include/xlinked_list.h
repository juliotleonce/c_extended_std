/**
 * @file xlinked_list.h
 * @brief A doubly linked list implementation.
 */

#ifndef EXTENDED_STD_XLINKED_LIST_H
#define EXTENDED_STD_XLINKED_LIST_H
#include <stddef.h>

/**
 * @brief Helper macro to define a type-safe alias for an XLinkedList of a specific type.
 * @param T The type of elements in the list.
 */
#define DEFINE_XLINKED_LIST_INNER(T) typedef XLinkedList* XLinkedList_of_##T;

/**
 * @brief Defines a type-safe alias for XLinkedList of type T.
 * @note This macro uses typedef. Use it once per file or in a shared header to avoid redefinition.
 */
#define DEFINE_XLINKED_LIST_OF(T) DEFINE_XLINKED_LIST_INNER(T)

/**
 * @brief Helper macro to refer to a type-defined XLinkedList of type T.
 * @param T The type of elements.
 */
#define XLinkedList_(T) XLinkedList_of_##T

/**
 * @struct XLinkedListIterator
 * @brief Opaque iterator for XLinkedList.
 */
typedef struct XLinkedListIterator XLinkedListIterator;

/**
 * @brief Function pointer type for moving the iterator to the next element.
 */
typedef void (*XLinkedListIteratorNextFn) (XLinkedListIterator *iterator);

/**
 * @struct XLinkedListNode
 * @brief Represents a node in the doubly linked list.
 */
typedef struct XLinkedListNode {
    void *value;                    /**< Pointer to the stored value. */
    struct XLinkedListNode *next;   /**< Pointer to the next node. */
    struct XLinkedListNode *prev;   /**< Pointer to the previous node. */
} XLinkedListNode;

/**
 * @struct XLinkedList
 * @brief Represents a linked list.
 */
typedef struct {
    XLinkedListNode *head;      /**< Pointer to the first node. */
    XLinkedListNode *tail;      /**< Pointer to the last node. */
    size_t value_type_size;     /**< Size of each element in bytes. */
    unsigned length;            /**< Number of elements in the list. */
} XLinkedList;

/**
 * @struct XLinkedListIterator
 * @brief Iterator structure for traversing XLinkedList.
 */
typedef struct XLinkedListIterator {
    XLinkedListNode *current;           /**< Current node being visited. */
    XLinkedListIteratorNextFn next_fn;  /**< Function to move to the next node. */
    unsigned index;                     /**< Current index of the iterator. */
} XLinkedListIterator;

/**
 * @brief Creates a new linked list.
 * @param value_type_size Size of each element in bytes.
 * @return A pointer to the newly created XLinkedList.
 */
XLinkedList *xlinked_list_new(size_t value_type_size);

/**
 * @brief Pushes an element to the end of the list.
 * @param xlinked_list Pointer to the XLinkedList.
 * @param value Pointer to the value to add.
 */
void xlinked_list_push(XLinkedList *xlinked_list, const void *value);

/**
 * @brief Returns an iterator starting at the head of the list.
 * @param xlinked_list Pointer to the XLinkedList.
 * @return An XLinkedListIterator initialized at the head.
 */
XLinkedListIterator xlinked_list_iterator(const XLinkedList *xlinked_list);

/**
 * @brief Returns a pointer to the value at the specified index.
 * @param xlinked_list Pointer to the XLinkedList.
 * @param index The index of the element.
 * @return A pointer to the value at the given index.
 */
void *xlinked_list_at(const XLinkedList *xlinked_list, unsigned index);

/**
 * @brief Moves the iterator to the next element.
 * @param iterator Pointer to the XLinkedListIterator.
 */
void xlinked_list_iterator_next(XLinkedListIterator *iterator);

/**
 * @brief Frees the memory associated with the XLinkedList.
 * @param xlinked_list Pointer to the XLinkedList to free.
 */
void xlinked_list_free(const XLinkedList *xlinked_list);

#endif //EXTENDED_STD_XLINKED_LIST_H