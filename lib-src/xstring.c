#include "include/xstring.h"
#include "include/xmemctl.h"
#include <ctype.h>
#include <string.h>


static void xstring_resize(XString *xstring, unsigned new_length);

XString *xstring_new(const char *str) {
    XString *xstring = xmem_alloc(sizeof(XString));
    const size_t str_length = strlen(str);
    const size_t capacity = str_length > 8 ?
        str_length + (str_length >> 1) : 8;

    xstring->length = str_length;
    xstring->capacity = capacity;
    xstring->c_str = xmem_alloc(xstring->capacity);

    strcpy(xstring->c_str, str);

    return xstring;
}

XString *xstring_copy(const XString *xstring) {
    return xstring_new(xstring->c_str);
}

XString *xstring_concat(XString *xstring, const XString *other) {
    return xstring_concat_c_str(xstring, other->c_str);
}

XString *xstring_concat_c_str(XString *xstring, const char *other) {
    const unsigned new_length = xstring->length + strlen(other);

    if (new_length > xstring->capacity) xstring_resize(xstring, new_length);
    xstring->length = new_length;
    strcat(xstring->c_str, other);

    return xstring;
}

XString *xstring_concat_char(XString *xstring, const char c) {
    const size_t new_length = xstring->length + 1;
    if (new_length > xstring->capacity) xstring_resize(xstring, new_length);

    xstring->length = new_length;
    xstring->c_str[new_length - 1] = c;
    xstring->c_str[new_length] = '\0';
    return xstring;
}

XString *xstring_substring(const XString *xstring, const unsigned start, const unsigned end) {
    XString *substring = xmem_alloc(sizeof(XString));

    substring->length = end - start;
    substring->capacity = substring->length + (substring->length >> 1);
    substring->c_str = xmem_alloc(substring->capacity);
    strncpy(substring->c_str, xstring->c_str + start, substring->length);
    substring->c_str[substring->length] = '\0';

    return substring;
}

XString *xstring_replace(XString *xstring, const char *old_str, const char *new_str) {
    const int old_str_index = xstring_find_first_index_of(xstring, old_str);

    if (old_str_index != -1) {
        const size_t old_str_length = strlen(old_str);
        const size_t second_part_offset = old_str_index + old_str_length;

        XString *temp = xstring_substring(xstring, 0, old_str_index);
        const XString *second_part = xstring_substring(xstring, second_part_offset, xstring->length);
        xstring_concat_c_str(temp, new_str);
        xstring_concat(temp, second_part);

        xmem_free(xstring->c_str);
        xstring->length = temp->length;
        xstring->c_str = temp->c_str;

        xmem_free(temp);
        xstring_free(second_part);

        return xstring;
    }

    return xstring;
}

XString *xstring_to_upper(XString *xstring) {
    for (unsigned i = 0; i < xstring->length; ++i)
        xstring->c_str[i] = (char) toupper(xstring->c_str[i]);
    return xstring;
}

XString *xstring_to_lower(XString *xstring) {
    for (unsigned i = 0; i < xstring->length; ++i)
        xstring->c_str[i] = (char) tolower(xstring->c_str[i]);
    return xstring;
}

XString *xstring_join(XArray_(XString) xarray, const char *separator) {
    XString *joined = xstring_new("");
    for (unsigned i = 0; i < xarray->length; ++i) {
        const XString *xstring_segments = xarray_at(xarray, i);
        joined = xstring_concat(joined, xstring_segments);
        if (i != xarray->length - 1) joined = xstring_concat_c_str(joined, separator);
    }
    return joined;
}

XArray *xstring_split(const XString *xstring, const char *separator) {
    XArray *split = xarray_new(sizeof(XString));
    unsigned separator_it = 0;
    unsigned xstring_it = 0;
    unsigned split_it = 0;
    const size_t separator_length = strlen(separator);

    while (xstring_it < xstring->length) {
        const char separator_char = separator[separator_it];
        const char xstring_char = xstring->c_str[xstring_it];

        if (separator_char - xstring_char == 0) {
            if (separator_it == separator_length - 1) {
                xarray_push(split, xstring_substring(xstring, split_it, xstring_it - separator_length + 1));
                split_it = xstring_it + separator_length;
                xstring_it = split_it;
                separator_it = 0;
                continue;
            }

            separator_it++;
            xstring_it++;
            continue;
        }

        if (separator_it == 0) xstring_it++;
        else separator_it = 0;

        if (xstring_it == xstring->length) {
            xarray_push(split, xstring_substring(xstring, split_it, xstring_it));
        }
    }

    return split;
}

char xstring_at(const XString *xstring, unsigned index) {
    if (xstring->length <= index) return '\0';
    return xstring->c_str[index];
}

int xstring_find_first_index_of(const XString *xstring, const char *substring) {
    unsigned sub_string_it = 0;
    unsigned xstring_it = 0;
    const size_t substring_length = strlen(substring);

    while (xstring_it < xstring->length) {
        const char sub_string_char = substring[sub_string_it];
        const char xstring_char = xstring->c_str[xstring_it];

        if (sub_string_char - xstring_char == 0) {
            if (sub_string_it == substring_length - 1)
                return (int) (xstring_it-substring_length+1);
            sub_string_it++;
            xstring_it++;
            continue;
        }

        if (sub_string_it == 0) xstring_it++;
        else sub_string_it = 0;
    }
    return -1;
}

bool xstring_equal(const XString *xstring, const XString *other) {
    if (xstring->length != other->length) return false;
    return strcmp(xstring->c_str, other->c_str) == 0;
}

bool xstring_equal_c_str(const XString *xstring, const char *other) {
    return strcmp(xstring->c_str, other) == 0;
}

bool xstring_contains(const XString *xstring, const char *substring) {
    return xstring_find_first_index_of(xstring, substring) != -1;
}

void xstring_free(const XString *xstring) {
    xmem_free(xstring->c_str);
    xmem_free(xstring);
}

void inline  xstring_resize(XString *xstring, const unsigned new_length) {
    xstring->length = new_length;
    while (xstring->capacity < xstring->length)
        xstring->capacity += xstring->capacity >> 1;
    xstring->c_str = xmem_realloc(xstring->c_str, xstring->capacity);
}


