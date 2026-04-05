#include "../include/xlinked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

XLinkedList *xlinked_list_new(const size_t value_type_size) {
    XLinkedList *xlinked_list = calloc(1, sizeof(XLinkedList));

    xlinked_list->head = NULL;
    xlinked_list->tail = NULL;
    xlinked_list->value_type_size = value_type_size;
    xlinked_list->length = 0;

    return xlinked_list;
}

void xlinked_list_push(XLinkedList *xlinked_list, const void *value) {
    XLinkedListNode *new_node = malloc(sizeof(XLinkedListNode));
    new_node->value = malloc(xlinked_list->value_type_size);
    memcpy(new_node->value, value, xlinked_list->value_type_size);
    new_node->next = NULL;

    if (xlinked_list->head == NULL) {
        xlinked_list->head = new_node;
        xlinked_list->tail = new_node;
    } else {
        xlinked_list->tail->next = new_node;
        xlinked_list->tail = new_node;
    }

    xlinked_list->length++;
}

XLinkedListIterator xlinked_list_iterator(const XLinkedList *xlinked_list) {
    XLinkedListIterator iterator;
    iterator.current = xlinked_list->head;
    iterator.next_fn = xlinked_list_iterator_next;
    iterator.index = 0;
    return iterator;
}

void * xlinked_list_at(const XLinkedList *xlinked_list, const unsigned index) {
    if (index >= xlinked_list->length) {
        printf("Index out of bounds\n");
        exit(1);
    }

    XLinkedListIterator it = xlinked_list_iterator(xlinked_list);
    while (it.index < index) xlinked_list_iterator_next(&it);

    return it.current->value;
}

void xlinked_list_iterator_next(XLinkedListIterator *iterator) {
    if (iterator->current == NULL) {
        printf("Iterator is already at the end\n");
        exit(1);
    }

    iterator->current = iterator->current->next;
    iterator->index++;
}

void xlinked_list_free(XLinkedList *xlinked_list) {
    XLinkedListIterator iterator = xlinked_list_iterator(xlinked_list);

    while (iterator.current != NULL) {
        free(iterator.current->value);
        XLinkedListNode *node_to_free = iterator.current;
        iterator.next_fn(&iterator);
        free(node_to_free);
    }

    free(xlinked_list);
}



