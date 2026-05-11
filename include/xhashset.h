/**
 * @file xhashset.h
 * @brief A hash set implementation.
 */

#ifndef EXTENDED_STD_XHASHSET_H
#define EXTENDED_STD_XHASHSET_H

/**
 * @brief Initial capacity for a newly created XHashSet.
 */
#define INITIAL_CAPACITY 8

/**
 * @brief Load factor threshold for resizing the hash set.
 */
#define LOAD_FACTOR 0.75f

/**
 * @brief Macro to create a new XHashSet.
 */
#define XHASHSET_NEW() xhashset_create()

#include <stdbool.h>
#include <stddef.h>
#include "xarray.h"

/**
 * @brief Function pointer type for hashing elements in the set.
 * @param capacity The current capacity of the set.
 * @param element Pointer to the element to hash.
 * @param type_size Size of the element in bytes.
 * @return The calculated hash value.
 */
typedef unsigned (*XHashSetHashFn) (unsigned capacity, const void *element, size_t type_size);

/**
 * @brief Function pointer type for comparing two elements for equality.
 * @param a Pointer to the first element.
 * @param b Pointer to the second element.
 * @param type_size Size of the elements in bytes.
 * @return true if elements are equal, false otherwise.
 */
typedef bool (*XHashSetEqualFn) (const void *a, const void *b, size_t type_size);

/**
 * @struct XHashSetItem
 * @brief Represents an item in the hash set.
 */
typedef struct{
    void *value;    /**< Pointer to the stored value. */
    unsigned psl;   /**< Probe Sequence Length for Robin Hood hashing. */
    bool is_taken;  /**< Flag indicating if the slot is occupied. */
} XHashSetItem;

/**
 * @struct XHashSet
 * @brief Represents a hash set.
 */
typedef struct{
    XHashSetItem *items;        /**< Array of items. */
    size_t type_size;           /**< Size of each element in bytes. */
    unsigned capacity;          /**< Total number of slots available. */
    unsigned items_account;     /**< Number of items currently in the set. */
    XHashSetEqualFn equal_fn;   /**< Function used for equality comparison. */
    XHashSetHashFn hash_fn;     /**< Function used for hashing. */
} XHashSet;

/**
 * @brief Creates a new hash set.
 * @param type_size Size of each element in bytes.
 * @param equal_fn Equality function.
 * @param hash_fn Hashing function.
 * @return A pointer to the newly created XHashSet.
 */
XHashSet *xhashset_new(size_t type_size, XHashSetEqualFn equal_fn, XHashSetHashFn hash_fn);

/**
 * @brief Adds an item to the hash set.
 * @param xhashset Pointer to the XHashSet.
 * @param item_value Pointer to the value to add.
 */
void xhashset_add(XHashSet *xhashset, const void *item_value);

/**
 * @brief Checks if an item exists in the hash set.
 * @param xhashset Pointer to the XHashSet.
 * @param item_value Pointer to the value to check.
 * @return true if the item is present, false otherwise.
 */
bool xhashset_has(const XHashSet *xhashset, const void *item_value);

/**
 * @brief Removes an item from the hash set.
 * @param xhashset Pointer to the XHashSet.
 * @param item_value Pointer to the value to remove.
 */
void xhashset_remove(XHashSet *xhashset, const void *item_value);

/**
 * @brief Computes the union of two hash sets.
 * @param xhashset_a Pointer to the first XHashSet.
 * @param xhashset_b Pointer to the second XHashSet.
 * @return A new XHashSet containing all unique elements from both sets.
 */
XHashSet *xhashset_union(const XHashSet *xhashset_a, const XHashSet *xhashset_b);

/**
 * @brief Computes the intersection of two hash sets.
 * @param xhashset_a Pointer to the first XHashSet.
 * @param xhashset_b Pointer to the second XHashSet.
 * @return A new XHashSet containing elements present in both sets.
 */
XHashSet *xhashset_intersection(const XHashSet *xhashset_a, const XHashSet *xhashset_b);

/**
 * @brief Computes the difference between two hash sets (A - B).
 * @param xhashset_a Pointer to the first XHashSet.
 * @param xhashset_b Pointer to the second XHashSet.
 * @return A new XHashSet containing elements present in A but not in B.
 */
XHashSet *xhashset_difference(const XHashSet *xhashset_a, const XHashSet *xhashset_b);

/**
 * @brief Returns an XArray containing all values in the set.
 * @param xhashset Pointer to the XHashSet.
 * @return A pointer to an XArray containing the values.
 */
XArray *xhashset_values(const XHashSet *xhashset);

/**
 * @brief Removes all items from the hash set.
 * @param xhashset Pointer to the XHashSet.
 */
void xhashset_clear(XHashSet *xhashset);

/**
 * @brief Default hashing function for general purposes.
 * @param capacity Current capacity of the set.
 * @param element Pointer to the element.
 * @param type_size Size of the element.
 * @return The calculated hash.
 */
unsigned xhashset_default_hash_fn(unsigned capacity, const void *element, size_t type_size);

/**
 * @brief Default equality function (memory comparison).
 * @param a Pointer to first element.
 * @param b Pointer to second element.
 * @param type_size Size of elements.
 * @return true if memories match, false otherwise.
 */
bool xhashset_default_equal_fn(const void *a, const void *b, size_t type_size);

/**
 * @brief Frees the memory associated with the XHashSet.
 * @param xhashset Pointer to the XHashSet to free.
 */
void xhashset_free(const XHashSet *xhashset);

#endif