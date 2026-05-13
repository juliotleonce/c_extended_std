/**
 * @file xarray.h
 * @brief A dynamic array implementation (vector).
 */

#ifndef EXTENDED_STD_XARRAY_H
#define EXTENDED_STD_XARRAY_H
#include <stdlib.h>

/**
 * @brief Helper macro to define a type-safe alias for an XArray of a specific type.
 * @param T The type of elements in the array.
 */
#define DEFINE_XARRAY_INNER(T) typedef XArray* XArray_of_##T;

/**
 * @brief Defines a type-safe alias for XArray of type T.
 * @note This macro uses typedef. Use it once per file or in a shared header to avoid redefinition.
 */
#define DEFINE_XARRAY_OF(T) DEFINE_XARRAY_INNER(T)

/**
 * @brief Helper macro to refer to a type-defined XArray of type T.
 * @param T The type of elements.
 */
#define XArray_(T) XArray_of_##T

/**
 * @brief Macro to create a new XArray with a specific element type.
 * @param ElementType The type of elements to be stored in the array.
 */
#define XARRAY_NEW(ElementType) xarray_new(sizeof(ElementType))

/**
 * @brief Initial capacity for a newly created XArray.
 */
#define XARRAY_INITIAL_CAPACITY 8

/**
 * @struct XArray
 * @brief Represents a dynamic array (vector).
 */
typedef struct XArray {
    unsigned length;    /**< Number of elements currently in the array. */
    size_t capacity;    /**< Total capacity (number of slots) of the array. */
    size_t type_size;   /**< Size of each element in bytes. */
    void *c_tab;        /**< Pointer to the underlying memory buffer. */
} XArray;

/**
 * @brief Creates a new dynamic array.
 * @param type_size The size of each element in bytes.
 * @return A pointer to the newly created XArray.
 */
XArray *xarray_new(size_t type_size);

/**
 * @brief Pushes an element to the end of the array.
 * @param xarray Pointer to the XArray.
 * @param data Pointer to the data to be added.
 */
void xarray_push(XArray *xarray, const void *data);

/**
 * @brief Pops the last element from the array.
 * @param xarray Pointer to the XArray.
 * @param output Pointer to memory where the popped element will be copied.
 */
void xarray_pop(XArray *xarray, void *output);

/**
 * @brief Returns a pointer to the element at the specified index.
 * @param xarray Pointer to the XArray.
 * @param index The index of the element.
 * @return A pointer to the element at the given index.
 */
void *xarray_at(const XArray *xarray, unsigned index);

/**
 * @brief Creates a copy of the XArray.
 * @param xarray Pointer to the XArray to copy.
 * @return A pointer to the new XArray copy.
 */
XArray *xarray_copy(const XArray *xarray);

/**
 * @brief Creates an XArray from a C-style array.
 * @param tab Pointer to the source C-style array.
 * @param length Number of elements in the source array.
 * @param type_size Size of each element in bytes.
 * @return A pointer to the newly created XArray.
 */
XArray *xarray_from_tab(const void *tab, unsigned length, size_t type_size);

/**
 * @brief Creates a slice of an existing XArray.
 * @param xarray Pointer to the source XArray.
 * @param start Starting index of the slice (inclusive).
 * @param end Ending index of the slice (exclusive).
 * @return A pointer to the new XArray containing the slice.
 */
XArray *xarray_slice(const XArray *xarray, unsigned start, unsigned end);

/**
 * @brief Frees the memory associated with the XArray.
 * @param xarray Pointer to the XArray to free.
 */
void xarray_free(const XArray *xarray);

#endif