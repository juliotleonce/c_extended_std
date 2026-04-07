#include "../include/xstring.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

XString *xstring_new(const char *str) {
    XString *xstring = malloc(sizeof(XString));

    xstring->length = strlen(str);
    xstring->c_str = malloc(xstring->length + 1);

    strcpy(xstring->c_str, str);

    return xstring;
}

XString *xstring_copy(const XString *xstring) {
    return xstring_new(xstring->c_str);
}

XString *xstring_concat(const XString *xstring, const XString *other) {
    return xstring_concat_c_str(xstring, other->c_str);
}

XString *xstring_concat_c_str(const XString *xstring, const char *other) {
    XString *concat = malloc(sizeof(XString));
    const unsigned new_length = xstring->length + strlen(other);

    concat->length = new_length;
    concat->c_str = malloc(new_length + 1);

    strcpy(concat->c_str, xstring->c_str);
    strcat(concat->c_str, other);

    return concat;
}

XString *xstring_substring(const XString *xstring, const unsigned start, const unsigned end) {
    XString *substring = malloc(sizeof(XString));

    substring->length = end - start;
    substring->c_str = malloc(substring->length + 1);
    strncpy(substring->c_str, xstring->c_str + start, substring->length);
    substring->c_str[substring->length] = '\0';

    return substring;
}

XString *xstring_replace(const XString *xstring, const char *old_str, const char *new_str) {
    const int old_str_index = xstring_find_first_index_of(xstring, old_str);

    if (old_str_index != -1) {
        const size_t old_str_length = strlen(old_str);
        const size_t second_part_offset = old_str_index + old_str_length;
        const XString *first_part = xstring_substring(xstring, 0, old_str_index);
        const XString *second_part = xstring_substring(xstring, second_part_offset, xstring->length);
        const XString *repaced_first_part = xstring_concat_c_str(first_part, new_str);
        XString *repaced_full_part = xstring_concat(repaced_first_part, second_part);
        return repaced_full_part;
    }

    return xstring_copy(xstring);
}

XString *xstring_to_upper(const XString *xstring) {
    XString *upper = xstring_copy(xstring);
    for (unsigned i = 0; i < upper->length; ++i)
        upper->c_str[i] = (char) toupper(upper->c_str[i]);
    return upper;
}

XString *xstring_to_lower(const XString *xstring) {
    XString *lower = xstring_copy(xstring);
    for (unsigned i = 0; i < lower->length; ++i)
        lower->c_str[i] = (char) tolower(lower->c_str[i]);
    return lower;
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

void xstring_free(XString *xstring) {
    free(xstring->c_str);
    free(xstring);
}


