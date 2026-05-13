/**
 * @file xstring.h
 * @brief A dynamic string implementation.
 */

#ifndef EXTENDED_STD_XSTRING_H
#define EXTENDED_STD_XSTRING_H
#include <stdbool.h>
#include <stddef.h>

#include "xarray.h"

/**
 * @struct XString
 * @brief Represents a dynamic string.
 */
typedef struct {
    unsigned length;    /**< Length of the string (excluding null terminator). */
    size_t capacity;    /**< Total capacity of the string buffer. */
    char *c_str;        /**< Pointer to the C-style null-terminated string. */
} XString;

DEFINE_XARRAY_OF(XString)

/**
 * @brief Creates a new XString from a C-style string.
 * @param str The source C-style string.
 * @return A pointer to the newly created XString.
 */
XString *xstring_new(const char *str);

/**
 * @brief Creates a copy of an XString.
 * @param xstring Pointer to the source XString.
 * @return A pointer to the new XString copy.
 */
XString *xstring_copy(const XString *xstring);

/**
 * @brief Concatenates two XStrings.
 * @param xstring Pointer to the first XString (will be modified).
 * @param other Pointer to the second XString to append.
 * @return Pointer to the modified first XString.
 */
XString *xstring_concat(XString *xstring, const XString *other);

/**
 * @brief Concatenates a C-style string to an XString.
 * @param xstring Pointer to the XString (will be modified).
 * @param other Pointer to the C-style string to append.
 * @return Pointer to the modified XString.
 */
XString *xstring_concat_c_str(XString *xstring, const char *other);

/**
 * @brief  Concatenates a char to an XString
 * @param xstring Pointer to the XString (will be modified).
 * @param c Char to concatenat
 * @return Point to the modified XString.
 */
XString *xstring_concat_char(XString *xstring, char c);

/**
 * @brief Creates a substring from an XString.
 * @param xstring Pointer to the source XString.
 * @param start Starting index (inclusive).
 * @param end Ending index (exclusive).
 * @return A pointer to the new XString containing the substring.
 */
XString *xstring_substring(const XString *xstring, unsigned start, unsigned end);

/**
 * @brief Replaces occurrences of a substring with another.
 * @param xstring Pointer to the XString (will be modified).
 * @param old_str Substring to find.
 * @param new_str Substring to replace with.
 * @return Pointer to the modified XString.
 */
XString *xstring_replace(XString *xstring, const char *old_str, const char *new_str);

/**
 * @brief Converts the XString to uppercase.
 * @param xstring Pointer to the XString (will be modified).
 * @return Pointer to the modified XString.
 */
XString *xstring_to_upper(XString *xstring);

/**
 * @brief Converts the XString to lowercase.
 * @param xstring Pointer to the XString (will be modified).
 * @return Pointer to the modified XString.
 */
XString *xstring_to_lower(XString *xstring);

/**
 * @brief Joins an XArray of strings into a single XString using a separator.
 * @param xarray Pointer to the XArray containing strings.
 * @param separator The string to insert between each element.
 * @return A pointer to the newly created XString.
 */
XString *xstring_join(XArray_(XString) xarray, const char *separator);

/**
 * @brief Splits an XString into an XArray of XStrings using a separator.
 * @param xstring Pointer to the XString.
 * @param separator The string to split by.
 * @return A pointer to a new XArray containing XString pointers.
 */
XArray_(XString) xstring_split(const XString *xstring, const char *separator);

/**
 * @brief Finds the first occurrence of a substring in the XString.
 * @param xstring Pointer to the XString.
 * @param substring The substring to find.
 * @return The index of the first occurrence, or -1 if not found.
 */
int xstring_find_first_index_of(const XString *xstring, const char *substring);

/**
 * @brief Checks if two XStrings are equal.
 * @param xstring Pointer to the first XString.
 * @param other Pointer to the second XString.
 * @return true if equal, false otherwise.
 */
bool xstring_equal(const XString *xstring, const XString *other);

/**
 * @brief Checks if an XString is equal to a C-style string.
 * @param xstring Pointer to the XString.
 * @param other Pointer to the C-style string.
 * @return true if equal, false otherwise.
 */
bool xstring_equal_c_str(const XString *xstring, const char *other);

/**
 * @brief Checks if an XString contains a substring.
 * @param xstring Pointer to the XString.
 * @param substring The substring to look for.
 * @return true if found, false otherwise.
 */
bool xstring_contains(const XString *xstring, const char *substring);

/**
 * @brief Frees the memory associated with the XString.
 * @param xstring Pointer to the XString to free.
 */
void xstring_free(const XString *xstring);

#endif