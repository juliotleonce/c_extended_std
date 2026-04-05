//
// Created by ajl420 on 4/5/26.
//

#ifndef EXTENDED_STD_XSTRING_H
#define EXTENDED_STD_XSTRING_H
#include <stdbool.h>

typedef struct {
    unsigned length;
    char *c_str;
} XString;

XString *xstring_new(const char *str);

XString *xstring_copy(const XString *xstring);

XString *xstring_concat(const XString *xstring, const XString *other);

XString *xstring_concat_c_str(const XString *xstring, const char *other);

XString *xstring_substring(const XString *xstring, unsigned start, unsigned end);

XString *xstring_replace(const XString *xstring, const char *old_str, const char *new_str);

XString *xstring_to_upper(const XString *xstring);

XString *xstring_to_lower(const XString *xstring);

unsigned xstring_find_first_index_of(const XString *xstring, const char *substring);

bool xstring_equal(const XString *xstring, const XString *other);

bool xstring_equal_c_str(const XString *xstring, const char *other);

bool xstring_contains(const XString *xstring, const char *substring);

void xstring_free(XString *xstring);

#endif //EXTENDED_STD_XSTRING_H