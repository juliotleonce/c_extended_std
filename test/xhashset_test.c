#include "include/xhashset.h"
#include <string.h>
#include "test_utils.h"

void test_xhashset_basic() {
    TEST_START("XHashSet Basic: New, Add, Has");

    XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int val1 = 10, val2 = 20, val3 = 30;
    xhashset_add(set, &val1);
    xhashset_add(set, &val2);

    if (!xhashset_has(set, &val1)) TEST_FAIL("Set should contain val1");
    if (!xhashset_has(set, &val2)) TEST_FAIL("Set should contain val2");
    if (xhashset_has(set, &val3)) TEST_FAIL("Set should not contain val3");
    if (set->items_account != 2) TEST_FAIL("Set count should be 2");

    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_remove() {
    TEST_START("XHashSet Remove");

    XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int values[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++) {
        xhashset_add(set, &values[i]);
    }

    int to_remove = 3;
    xhashset_remove(set, &to_remove);

    if (xhashset_has(set, &to_remove)) TEST_FAIL("Set should not contain removed value");
    if (set->items_account != 4) TEST_FAIL("Set count should be 4 after removal");

    int not_in_set = 10;
    xhashset_remove(set, &not_in_set);
    if (set->items_account != 4) TEST_FAIL("Set count should still be 4 after removing non-existent value");

    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_clear() {
    TEST_START("XHashSet Clear");

    XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int val = 42;
    xhashset_add(set, &val);
    xhashset_clear(set);

    if (set->items_account != 0) TEST_FAIL("Set should be empty after clear");
    if (xhashset_has(set, &val)) TEST_FAIL("Set should not contain value after clear");

    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_union() {
    TEST_START("XHashSet Union");

    XHashSet *set_a = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
    XHashSet *set_b = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int a1 = 1, a2 = 2, common = 3, b1 = 4;
    xhashset_add(set_a, &a1);
    xhashset_add(set_a, &a2);
    xhashset_add(set_a, &common);

    xhashset_add(set_b, &common);
    xhashset_add(set_b, &b1);

    XHashSet *set_union = xhashset_union(set_a, set_b);

    if (set_union->items_account != 4) TEST_FAIL("Union should have 4 elements");
    if (!xhashset_has(set_union, &a1)) TEST_FAIL("Union missing a1");
    if (!xhashset_has(set_union, &common)) TEST_FAIL("Union missing common");
    if (!xhashset_has(set_union, &b1)) TEST_FAIL("Union missing b1");

    xhashset_free(set_a);
    xhashset_free(set_b);
    xhashset_free(set_union);
    TEST_PASS();
}

void test_xhashset_intersection() {
    TEST_START("XHashSet Intersection");

    XHashSet *set_a = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
    XHashSet *set_b = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int a1 = 1, common = 3, b1 = 4;
    xhashset_add(set_a, &a1);
    xhashset_add(set_a, &common);

    xhashset_add(set_b, &common);
    xhashset_add(set_b, &b1);

    XHashSet *set_inter = xhashset_intersection(set_a, set_b);

    if (set_inter->items_account != 1) TEST_FAIL("Intersection should have 1 element");
    if (!xhashset_has(set_inter, &common)) TEST_FAIL("Intersection missing common element");
    if (xhashset_has(set_inter, &a1)) TEST_FAIL("Intersection should not have a1");

    xhashset_free(set_a);
    xhashset_free(set_b);
    xhashset_free(set_inter);
    TEST_PASS();
}

void test_xhashset_difference() {
    TEST_START("XHashSet Difference");

    XHashSet *set_a = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
    XHashSet *set_b = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int a1 = 1, common = 3, b1 = 4;
    xhashset_add(set_a, &a1);
    xhashset_add(set_a, &common);

    xhashset_add(set_b, &common);
    xhashset_add(set_b, &b1);

    XHashSet *set_diff = xhashset_difference(set_a, set_b);

    if (set_diff->items_account != 1) TEST_FAIL("Difference should have 1 element");
    if (!xhashset_has(set_diff, &a1)) TEST_FAIL("Difference missing a1");
    if (xhashset_has(set_diff, &common)) TEST_FAIL("Difference should not have common element");

    xhashset_free(set_a);
    xhashset_free(set_b);
    xhashset_free(set_diff);
    TEST_PASS();
}

void test_xhashset_values() {
    TEST_START("XHashSet Values (to XArray)");

    XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
    int vals[] = {10, 20, 30};
    for(int i=0; i<3; i++) xhashset_add(set, &vals[i]);

    XArray *arr = xhashset_values(set);
    if (arr->length != 3) TEST_FAIL("Array size should be 3");

    // Check if all values are present in array
    for(int i=0; i<3; i++) {
        bool found = false;
        for(unsigned j=0; j<arr->length; j++) {
            if (*(int*)xarray_at(arr, j) == vals[i]) {
                found = true;
                break;
            }
        }
        if (!found) TEST_FAIL("Value missing in XArray");
    }

    xarray_free(arr);
    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_duplicates() {
    TEST_START("XHashSet Duplicates");

    XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int val = 100;
    xhashset_add(set, &val);
    xhashset_add(set, &val);
    xhashset_add(set, &val);

    if (set->items_account != 1) TEST_FAIL("Set should only contain 1 element after adding duplicates");

    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_resize() {
    TEST_START("XHashSet Resize");

    XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
    
    int num_elements = 100;
    for (int i = 0; i < num_elements; i++) {
        xhashset_add(set, &i);
    }

    if (set->items_account != (unsigned)num_elements) TEST_FAIL("Set should contain all elements after resize");
    if (set->capacity <= INITIAL_CAPACITY) TEST_FAIL("Set should have resized");

    for (int i = 0; i < num_elements; i++) {
        if (!xhashset_has(set, &i)) TEST_FAIL("Set missing element after resize");
    }

    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_strings() {
    TEST_START("XHashSet Strings");

    XHashSet *set = xhashset_new(sizeof(char*), xhashset_default_equal_fn, xhashset_default_hash_fn);

    char *s1 = "hello";
    char *s2 = "world";
    char *s3 = "hello"; // Same content, different pointer? No, string literals are pooled usually.

    xhashset_add(set, &s1);
    xhashset_add(set, &s2);
    xhashset_add(set, &s3);

    if (set->items_account != 2) TEST_FAIL("Set should contain 2 unique strings");
    if (!xhashset_has(set, &s1)) TEST_FAIL("Set missing 'hello'");
    if (!xhashset_has(set, &s2)) TEST_FAIL("Set missing 'world'");

    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_empty_operations() {
    TEST_START("XHashSet Empty Operations");

    XHashSet *set_a = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
    XHashSet *set_b = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);

    int val = 1;
    xhashset_add(set_a, &val);

    SECTION("Union with empty");
    XHashSet *u = xhashset_union(set_a, set_b);
    if (u->items_account != 1) TEST_FAIL("Union with empty set failed");
    xhashset_free(u);

    SECTION("Intersection with empty");
    XHashSet *i = xhashset_intersection(set_a, set_b);
    if (i->items_account != 0) TEST_FAIL("Intersection with empty set should be empty");
    xhashset_free(i);

    SECTION("Difference with empty");
    XHashSet *d = xhashset_difference(set_a, set_b);
    if (d->items_account != 1) TEST_FAIL("Difference with empty set failed");
    xhashset_free(d);

    xhashset_free(set_a);
    xhashset_free(set_b);
    TEST_PASS();
}

void test_xhashset_stress() {
    TEST_START("XHashSet Stress Test");

    XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
    const int num_elements = 10000;

    SECTION("Adding elements");
    for (int i = 0; i < num_elements; i++) {
        xhashset_add(set, &i);
    }

    if (set->items_account != (unsigned)num_elements) TEST_FAIL("Stress: item count mismatch after additions");

    SECTION("Checking presence");
    for (int i = 0; i < num_elements; i++) {
        if (!xhashset_has(set, &i)) TEST_FAIL("Stress: missing element");
    }

    SECTION("Removing half");
    for (int i = 0; i < num_elements; i += 2) {
        xhashset_remove(set, &i);
    }
    if (set->items_account != (unsigned)num_elements / 2) TEST_FAIL("Stress: item count mismatch after partial removal");

    SECTION("Checking remaining elements");
    for (int i = 0; i < num_elements; i++) {
        bool should_be_present = (i % 2 != 0);
        if (xhashset_has(set, &i) != should_be_present) {
            if (should_be_present) TEST_FAIL("Stress: missing odd element");
            else TEST_FAIL("Stress: even element still present");
        }
    }

    xhashset_free(set);
    TEST_PASS();
}

void test_xhashset_memory_cycles() {
    TEST_START("XHashSet Memory Cycles Test");

    const int cycles = 100;

    for (int c = 0; c < cycles; c++) {
        const int elements_per_cycle = 500;
        XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
        for (int i = 0; i < elements_per_cycle; i++) {
            xhashset_add(set, &i);
        }
        xhashset_clear(set);
        if (set->items_account != 0) TEST_FAIL("Memory Cycles: items_account not 0 after clear");

        for (int i = 0; i < elements_per_cycle; i++) {
            xhashset_add(set, &i);
        }
        xhashset_free(set);
    }

    TEST_PASS();
}

void run_all_xhashset_tests() {
    SECTION("XHASHSET TESTS");
    test_xhashset_basic();
    test_xhashset_remove();
    test_xhashset_clear();
    test_xhashset_union();
    test_xhashset_intersection();
    test_xhashset_difference();
    test_xhashset_values();
    test_xhashset_duplicates();
    test_xhashset_resize();
    test_xhashset_strings();
    test_xhashset_empty_operations();
    test_xhashset_stress();
    test_xhashset_memory_cycles();
}
