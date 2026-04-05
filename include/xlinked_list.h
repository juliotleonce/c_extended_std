//
// Created by ajl420 on 4/5/26.
//

#ifndef EXTENDED_STD_XLINKED_LIST_H
#define EXTENDED_STD_XLINKED_LIST_H
#include <stddef.h>

typedef struct XLinkedListIterator XLinkedListIterator;

typedef void (*XLinkedListIteratorNextFn) (XLinkedListIterator *iterator);

typedef struct XLinkedListNode {
    void *value;
    struct XLinkedListNode *next;
} XLinkedListNode;

typedef struct {
    XLinkedListNode *head;
    XLinkedListNode *tail;
    size_t value_type_size;
    unsigned length;
} XLinkedList;

typedef struct XLinkedListIterator {
    XLinkedListNode *current;
    XLinkedListIteratorNextFn next_fn;
    unsigned index;
} XLinkedListIterator;

XLinkedList *xlinked_list_new(size_t value_type_size);

void xlinked_list_push(XLinkedList *xlinked_list, const void *value);

XLinkedListIterator xlinked_list_iterator(const XLinkedList *xlinked_list);

void xlinked_list_iterator_next(XLinkedListIterator *iterator);

void xlinked_list_free(XLinkedList *xlinked_list);

#endif //EXTENDED_STD_XLINKED_LIST_H