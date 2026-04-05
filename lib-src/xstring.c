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

bool xstring_equal(const XString *xstring, const XString *other) {
    if (xstring->length != other->length) return false;
    return strcmp(xstring->c_str, other->c_str) == 0;
}

bool xstring_equal_c_str(const XString *xstring, const char *other) {
    return strcmp(xstring->c_str, other) == 0;
}

void xstring_free(XString *xstring) {
    free(xstring->c_str);
    free(xstring);
}


