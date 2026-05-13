/**
 * @file xhashmap.h
 * @brief A hash map implementation with string keys.
 */

#ifndef EXTENDED_STD_XHASHMAP_H
#define EXTENDED_STD_XHASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include "xarray.h"

/**
 * @brief Initial capacity for a newly created XHashMap.
 */
#define INITIAL_CAPACITY 8

/**
 * @brief Load factor threshold for resizing the hash map.
 */
#define LOAD_FACTOR 0.75f

/**
 * @brief Helper macro to define a type-safe alias for an XHashMap of a specific value type.
 * @param T The type of values in the map.
 */
#define DEFINE_XHASHMAP_INNER(T) typedef XHashMap* XHashMap_of_##T;

/**
 * @brief Defines a type-safe alias for XHashMap of value type T.
 * @note This macro uses typedef. Use it once per file or in a shared header to avoid redefinition.
 */
#define DEFINE_XHASHMAP_OF(T) DEFINE_XHASHMAP_INNER(T)

/**
 * @brief Helper macro to refer to a type-defined XHashMap of value type T.
 * @param T The type of values.
 */
#define XHashMap_(T) XHashMap_of_##T

/**
 * @brief Macro to create a new XHashMap with a specific value type.
 * @param ItemsValueType The type of values to be stored in the map.
 */
#define XHASHMAP_NEW(ItemsValueType) xhashmap_new(sizeof(ItemsValueType))

/**
 * @struct XHashMapEntry
 * @brief Represents an entry in the hash map.
 */
typedef struct {
    char *key;      /**< The string key. */
    void *value;    /**< Pointer to the value. */
    bool is_taken;  /**< Flag indicating if the entry is occupied. */
    unsigned psl;   /**< Probe Sequence Length for Robin Hood hashing. */
} XHashMapEntry;

/**
 * @struct XHashMap
 * @brief Represents a hash map.
 */
typedef struct {
    XHashMapEntry *entries; /**< Array of entries. */
    unsigned capacity;      /**< Total number of slots available. */
    unsigned items_count;   /**< Number of items currently in the map. */
    size_t type_size;       /**< Size of each value in bytes. */
} XHashMap;

/**
 * @brief Creates a new hash map.
 * @param type_size The size of each value in bytes.
 * @return A pointer to the newly created XHashMap.
 */
XHashMap *xhashmap_new(size_t type_size);

/**
 * @brief Inserts or updates a key-value pair in the map.
 * @param xhashmap Pointer to the XHashMap.
 * @param key The string key.
 * @param value Pointer to the value to store.
 */
void xhashmap_put(XHashMap *xhashmap, const char *key, const void *value);

/**
 * @brief Returns an XArray containing all keys in the map.
 * @param xhashmap Pointer to the XHashMap.
 * @return A pointer to an XArray containing strings.
 */
XArray *xhashmap_keys(const XHashMap *xhashmap);

/**
 * @brief Returns an XArray containing all values in the map.
 * @param xhashmap Pointer to the XHashMap.
 * @return A pointer to an XArray containing the values.
 */
XArray *xhashmap_values(const XHashMap *xhashmap);

/**
 * @brief Removes an entry from the map by its key.
 * @param xhashmap Pointer to the XHashMap.
 * @param key The key to remove.
 */
void xhashmap_remove(XHashMap *xhashmap, const char *key);

/**
 * @brief Retrieves a value from the map by its key.
 * @param xhashmap Pointer to the XHashMap.
 * @param key The key to look for.
 * @return A pointer to the value, or NULL if not found.
 */
void *xhashmap_get(const XHashMap *xhashmap, const char *key);

/**
 * @brief Frees the memory associated with the XHashMap.
 * @param xhashmap Pointer to the XHashMap to free.
 */
void xhashmap_free(const XHashMap *xhashmap);

/**
 * @brief Converts map keys to a C-style array of strings.
 * @param xhashmap Pointer to the XHashMap.
 * @param out_count Pointer to an unsigned int to store the number of keys.
 * @return A C-style array of strings.
 */
char **keys_to_array(const XHashMap *xhashmap, unsigned *out_count);

#endif //EXTENDED_STD_XHASHMAP_H