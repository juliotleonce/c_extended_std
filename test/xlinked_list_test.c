#include "../include/xlinked_list.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "test_utils.h"

void test_linked_list_basic() {
    TEST_START("XLinkedList Basic Operations");

    XLinkedList *list = xlinked_list_new(sizeof(int));
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->head == NULL);
    assert(list->tail == NULL);

    int val1 = 10, val2 = 20, val3 = 30;
    xlinked_list_push(list, &val1);
    assert(list->length == 1);
    assert(*(int*)list->head->value == 10);
    assert(*(int*)list->tail->value == 10);

    xlinked_list_push(list, &val2);
    assert(list->length == 2);
    assert(*(int*)list->head->value == 10);
    assert(*(int*)list->tail->value == 20);

    xlinked_list_push(list, &val3);
    assert(list->length == 3);
    assert(*(int*)list->tail->value == 30);

    xlinked_list_free(list);
    TEST_PASS();
}

void test_linked_list_iterator() {
    TEST_START("XLinkedList Iterator");

    XLinkedList *list = xlinked_list_new(sizeof(int));
    for (int i = 0; i < 5; i++) {
        xlinked_list_push(list, &i);
    }

    XLinkedListIterator it = xlinked_list_iterator(list);
    int count = 0;
    while (it.current != NULL) {
        assert(*(int*)it.current->value == count);
        assert(it.index == (unsigned)count);
        xlinked_list_iterator_next(&it);
        count++;
    }
    assert(count == 5);

    xlinked_list_free(list);
    TEST_PASS();
}

void test_linked_list_large_data() {
    TEST_START("XLinkedList Large Data (Stress Test)");
    
    typedef struct {
        char name[100];
        int id;
        double score;
    } Student;

    XLinkedList *list = xlinked_list_new(sizeof(Student));
    const int num_students = 1000;

    for (int i = 0; i < num_students; i++) {
        Student s;
        snprintf(s.name, sizeof(s.name), "Student %d", i);
        s.id = i;
        s.score = (double)i * 1.5;
        xlinked_list_push(list, &s);
    }

    assert(list->length == (unsigned)num_students);

    XLinkedListIterator it = xlinked_list_iterator(list);
    int i = 0;
    while (it.current != NULL) {
        Student *s = (Student*)it.current->value;
        char expected_name[100];
        snprintf(expected_name, sizeof(expected_name), "Student %d", i);
        
        assert(s->id == i);
        assert(strcmp(s->name, expected_name) == 0);
        assert(s->score == (double)i * 1.5);
        
        xlinked_list_iterator_next(&it);
        i++;
    }

    xlinked_list_free(list);
    TEST_PASS();
}

void test_linked_list_strings() {
    TEST_START("XLinkedList with Strings");

    XLinkedList *list = xlinked_list_new(sizeof(char*));
    char *strs[] = {"Hello", "World", "C", "Programming", "Extended", "Std"};
    int n = sizeof(strs) / sizeof(strs[0]);

    for (int i = 0; i < n; i++) {
        xlinked_list_push(list, &strs[i]);
    }

    XLinkedListIterator it = xlinked_list_iterator(list);
    int i = 0;
    while (it.current != NULL) {
        char *s = *(char**)it.current->value;
        assert(strcmp(s, strs[i]) == 0);
        xlinked_list_iterator_next(&it);
        i++;
    }

    xlinked_list_free(list);
    TEST_PASS();
}

void run_all_xlinked_list_tests() {
    SECTION("XLinkedList Tests");
    test_linked_list_basic();
    test_linked_list_iterator();
    test_linked_list_large_data();
    test_linked_list_strings();
}
