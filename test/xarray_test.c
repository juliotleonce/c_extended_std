#include "include/xarray.h"

#include <string.h>

#include "test_utils.h"

static void test_basic_new_and_push(void);
static void test_multiple_insertions(void);
static void test_get_by_index(void);
static void test_struct_values(void);
static void test_edge_cases(void);
static void test_memory_safety(void);
static void test_stress(void);
static void test_xarray_copy(void);
static void test_xarray_from_tab(void);


void run_all_xarray_tests(void) {
    print_separator();
    printf("\033[0;36m  XARRAY TEST SUITE\033[0m\n");
    printf("\033[0;36m  ===================\033[0m\n");
    print_separator();

    printf("\n\033[1;34mRunning tests...\033[0m\n");

    // Run all tests
    test_basic_new_and_push();
    print_separator();

    test_multiple_insertions();
    print_separator();

    test_get_by_index();
    print_separator();

    test_struct_values();
    print_separator();

    test_edge_cases();
    print_separator();

    test_memory_safety();
    print_separator();

    test_stress();
    print_separator();

    test_xarray_copy();
    print_separator();

    test_xarray_from_tab();
    print_separator();


    printf("\n");
    printf("\033[0;32m========================================\033[0m\n");
    printf("\033[0;32m  ALL TESTS COMPLETED\033[0m\n");
    printf("\033[0;32m========================================\033[0m\n");
    printf("\n");
}

// ========================================
// Test 1: Basic Creation and Single Push
// ========================================
static void test_basic_new_and_push(void) {
    TEST_START("Basic Creation & Single Push");

    XArray *array = XARRAY_NEW(int);

    SECTION("Creating array with type_size=sizeof(int)");
    printf("  array = %p\n", (void*)array);
    printf("  Expected: non-NULL pointer\n");

    if (array == NULL) {
        TEST_FAIL("Failed to create array");
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - Array created successfully\n");

    SECTION("Pushing single element");
    const int value = 42;
    printf("  Pushing: value=%d\n", value);
    xarray_push(array, &value);

    SECTION("Verifying array length");
    printf("  Array length: %u\n", array->length);
    if (array->length == 1) {
        printf("  Result: \033[0;32mOK\033[0m - Length is correct!\n");
    } else {
        TEST_FAIL("Length is incorrect");
        xarray_free(array);
        return;
    }

    SECTION("Retrieving value at index 0");
    int *retrieved = (int*)xarray_at(array, 0);
    printf("  Retrieved: %d\n", *retrieved);
    if (*retrieved == 42) {
        printf("  Result: \033[0;32mOK\033[0m - Value matches!\n");
        TEST_PASS();
    } else {
        TEST_FAIL("Failed to retrieve correct value");
    }

    xarray_free(array);
    printf("  Array freed successfully\n");

}

// ========================================
// Test 8: XArray Copy
// ========================================
static void test_xarray_copy(void) {
    TEST_START("XArray Copy");

    XArray *original = XARRAY_NEW(int);
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        xarray_push(original, &values[i]);
    }

    SECTION("Copying original array");
    XArray *copy = xarray_copy(original);

    SECTION("Verifying copy content and independence");
    if (copy == NULL) {
        TEST_FAIL("Failed to copy array");
        xarray_free(original);
        return;
    }

    if (copy->length != original->length) {
        TEST_FAIL("Copy length mismatch");
        xarray_free(original);
        xarray_free(copy);
        return;
    }

    for (unsigned i = 0; i < copy->length; i++) {
        int *orig_val = (int*)xarray_at(original, i);
        int *copy_val = (int*)xarray_at(copy, i);
        if (*orig_val != *copy_val) {
            TEST_FAIL("Copy value mismatch");
            xarray_free(original);
            xarray_free(copy);
            return;
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - Values match!\n");

    SECTION("Modifying copy and checking independence");
    int new_val = 99;
    xarray_push(copy, &new_val);
    if (copy->length == original->length + 1) {
        printf("  Result: \033[0;32mOK\033[0m - Copy is independent!\n");
    } else {
        TEST_FAIL("Copy is not independent");
        xarray_free(original);
        xarray_free(copy);
        return;
    }

    xarray_free(original);
    xarray_free(copy);
    TEST_PASS();
}

// ========================================
// Test 9: XArray From Tab
// ========================================
static void test_xarray_from_tab(void) {
    TEST_START("XArray From Tab");

    SECTION("Creating XArray from C array (int tab)");
    int tab[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    unsigned length = sizeof(tab) / sizeof(tab[0]);

    XArray *array = xarray_from_tab(tab, length, sizeof(int));

    if (array == NULL) {
        TEST_FAIL("Failed to create array from tab");
        return;
    }

    SECTION("Verifying length and content");
    if (array->length != length) {
        TEST_FAIL("Length mismatch after xarray_from_tab");
        xarray_free(array);
        return;
    }

    for (unsigned i = 0; i < length; i++) {
        int *val = (int*)xarray_at(array, i);
        if (*val != tab[i]) {
            TEST_FAIL("Value mismatch in array from tab");
            xarray_free(array);
            return;
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - All values from tab match!\n");

    SECTION("Modifying original tab and checking independence");
    tab[0] = 999;
    int *val_after = (int*)xarray_at(array, 0);
    if (*val_after == 1) {
        printf("  Result: \033[0;32mOK\033[0m - XArray is independent from original tab!\n");
    } else {
        TEST_FAIL("XArray is not independent from original tab");
        xarray_free(array);
        return;
    }

    xarray_free(array);
    TEST_PASS();
}

// ========================================
// Test 2: Multiple Insertions
// ========================================
static void test_multiple_insertions(void) {
    TEST_START("Multiple Insertions");

    XArray *array = XARRAY_NEW(double);

    SECTION("Pushing multiple elements");

    const double values[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10};
    const size_t num_values = sizeof(values) / sizeof(values[0]);

    printf("  Pushing %zu double values:\n", num_values);
    for (size_t i = 0; i < num_values; i++) {
        xarray_push(array, &values[i]);
        printf("    [%zu] value=%.2f\n", i, values[i]);
    }

    SECTION("Verifying array length");
    printf("  Array length: %u (expected: %zu)\n", array->length, num_values);
    if (array->length != num_values) {
        TEST_FAIL("Array length mismatch");
        xarray_free(array);
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - Length is correct!\n");

    SECTION("Verifying all values");
    printf("  Retrieving and verifying all values:\n");
    int all_ok = 1;
    for (size_t i = 0; i < num_values; i++) {
        const double *retrieved = (double*)xarray_at(array, (unsigned)i);
        if (*retrieved == values[i]) {
            printf("    \033[0;32m✓\033[0m [%zu] %.2f [OK]\n", i, *retrieved);
        } else {
            printf("    \033[0;31m✗\033[0m [%zu] expected %.2f, got %.2f\n",
                   i, values[i], *retrieved);
            all_ok = 0;
        }
    }

    if (all_ok) {
        TEST_PASS();
    } else {
        TEST_FAIL("Some values did not match");
    }

    xarray_free(array);
}

// ========================================
// Test 3: Get Element by Index
// ========================================
static void test_get_by_index(void) {
    TEST_START("Get Element by Index");

    XArray *array = XARRAY_NEW(int);

    SECTION("Pushing sequential values");
    const int values[] = {10, 20, 30, 40, 50};
    const size_t num_values = sizeof(values) / sizeof(values[0]);

    for (size_t i = 0; i < num_values; i++) {
        xarray_push(array, &values[i]);
    }
    printf("  Pushed %zu values\n", num_values);

    SECTION("Testing valid indices");
    printf("  Accessing elements at various indices:\n");
    int all_ok = 1;

    // Test first element
    int *first = (int*)xarray_at(array, 0);
    printf("    [0] = %d %s\n", *first, (*first == 10) ? "\033[0;32m[OK]\033[0m" : "\033[0;31m[FAIL]\033[0m");
    if (*first != 10) all_ok = 0;

    // Test middle element
    int *middle = (int*)xarray_at(array, 2);
    printf("    [2] = %d %s\n", *middle, (*middle == 30) ? "\033[0;32m[OK]\033[0m" : "\033[0;31m[FAIL]\033[0m");
    if (*middle != 30) all_ok = 0;

    // Test last element
    int *last = (int*)xarray_at(array, (unsigned)(num_values - 1));
    printf("    [%zu] = %d %s\n", num_values - 1, *last, (*last == 50) ? "\033[0;32m[OK]\033[0m" : "\033[0;31m[FAIL]\033[0m");
    if (*last != 50) all_ok = 0;

    // Test modifying value through pointer
    SECTION("Modifying value through get pointer");
    int *elem = (int*)xarray_at(array, 1);
    *elem = 999;
    printf("  Modified element at index 1 to %d\n", *elem);

    int *updated = (int*)xarray_at(array, 1);
    printf("  Retrieved again: %d\n", *updated);
    if (*updated == 999) {
        printf("  Result: \033[0;32mOK\033[0m - Value modified successfully!\n");
    } else {
        printf("  Result: \033[0;31mFAIL\033[0m - Value not modified\n");
        all_ok = 0;
    }

    if (all_ok) {
        TEST_PASS();
    } else {
        TEST_FAIL("Index access failed");
    }

    xarray_free(array);
}

// ========================================
// Test 4: Complex Data Structures (Struct Values)
// ========================================
static void test_struct_values(void) {
    TEST_START("Complex Data Structures (Struct Values)");

    typedef struct {
        int id;
        char name[32];
        float score;
    } User;

    XArray *array = XARRAY_NEW(User);

    SECTION("Creating users");

    User user1 = {.id = 1, .name = "Alice", .score = 95.5f};
    User user2 = {.id = 2, .name = "Bob", .score = 87.3f};
    User user3 = {.id = 3, .name = "Charlie", .score = 92.1f};

    printf("  Creating users:\n");
    printf("    User 1: id=%d, name=\"%s\", score=%.1f\n", user1.id, user1.name, user1.score);
    printf("    User 2: id=%d, name=\"%s\", score=%.1f\n", user2.id, user2.name, user2.score);
    printf("    User 3: id=%d, name=\"%s\", score=%.1f\n", user3.id, user3.name, user3.score);

    SECTION("Pushing users into array");
    xarray_push(array, &user1);
    printf("  Pushed: user at index 0\n");
    xarray_push(array, &user2);
    printf("  Pushed: user at index 1\n");
    xarray_push(array, &user3);
    printf("  Pushed: user at index 2\n");

    SECTION("Verifying all users");
    int all_ok = 1;

    const User originals[] = {user1, user2, user3};

    for (size_t i = 0; i < 3; i++) {
        User *retrieved = xarray_at(array, (unsigned)i);
        if (retrieved != NULL) {
            printf("  Retrieved user at index %zu:\n", i);
            printf("    id=%d, name=\"%s\", score=%.1f\n",
                   retrieved->id, retrieved->name, retrieved->score);

            if (retrieved->id == originals[i].id &&
                strcmp(retrieved->name, originals[i].name) == 0 &&
                retrieved->score == originals[i].score) {
                printf("    \033[0;32m✓\033[0m Match!\n");
            } else {
                printf("    \033[0;31m✗\033[0m Data mismatch!\n");
                all_ok = 0;
            }
        } else {
            printf("  \033[0;31m✗\033[0m Index %zu -> NULL\n", i);
            all_ok = 0;
        }
    }

    if (all_ok) {
        TEST_PASS();
    } else {
        TEST_FAIL("Struct retrieval failed");
    }

    xarray_free(array);
}

// ========================================
// Test 5: Edge Cases (Various Data Types)
// ========================================
static void test_edge_cases(void) {
    TEST_START("Edge Cases (Various Data Types)");

    SECTION("Test 1: Single character");
    XArray *char_array = xarray_new(sizeof(char));
    const char chars[] = {'A', 'B', 'C', 'D'};
    for (size_t i = 0; i < 4; i++) {
        xarray_push(char_array, &chars[i]);
    }
    printf("  Pushed 4 characters\n");
    int char_ok = 1;
    for (size_t i = 0; i < 4; i++) {
        const char *c = (char*)xarray_at(char_array, (unsigned)i);
        if (*c != chars[i]) char_ok = 0;
        printf("    [%zu] '%c' %s\n", i, *c, (*c == chars[i]) ? "\033[0;32m[OK]\033[0m" : "\033[0;31m[FAIL]\033[0m");
    }
    xarray_free(char_array);
    if (!char_ok) {
        TEST_FAIL("Char array test failed");
        return;
    }
    printf("  \033[0;32m✓\033[0m Char test passed\n");

    SECTION("Test 2: Long integers");
    XArray *long_array = XARRAY_NEW(long);
    const long longs[] = {0L, -1L, 1234567890123L, -9876543210L};
    for (size_t i = 0; i < 4; i++) {
        xarray_push(long_array, &longs[i]);
    }
    printf("  Pushed 4 long values\n");
    int long_ok = 1;
    for (size_t i = 0; i < 4; i++) {
        const long *l = (long*)xarray_at(long_array, (unsigned)i);
        if (*l != longs[i]) long_ok = 0;
        printf("    [%zu] %ld %s\n", i, *l, (*l == longs[i]) ? "\033[0;32m[OK]\033[0m" : "\033[0;31m[FAIL]\033[0m");
    }
    xarray_free(long_array);
    if (!long_ok) {
        TEST_FAIL("Long array test failed");
        return;
    }
    printf("  \033[0;32m✓\033[0m Long test passed\n");

    SECTION("Test 3: Strings (char arrays)");
    XArray *str_array = XARRAY_NEW(char[32]);
    const char *strings[] = {"Hello", "World", "Test", "XArray"};
    for (size_t i = 0; i < 4; i++) {
        xarray_push(str_array, strings[i]);
    }
    printf("  Pushed 4 strings\n");
    int str_ok = 1;
    for (size_t i = 0; i < 4; i++) {
        char *s = xarray_at(str_array, (unsigned)i);
        if (strcmp(s, strings[i]) != 0) str_ok = 0;
        printf("    [%zu] \"%s\" %s\n", i, s, (strcmp(s, strings[i]) == 0) ? "\033[0;32m[OK]\033[0m" : "\033[0;31m[FAIL]\033[0m");
    }
    xarray_free(str_array);
    if (!str_ok) {
        TEST_FAIL("String array test failed");
        return;
    }
    printf("  \033[0;32m✓\033[0m String test passed\n");

    SECTION("Test 4: Empty array operations");
    XArray *empty = XARRAY_NEW(int);
    printf("  Created empty array, length: %u\n", empty->length);
    if (empty->length == 0) {
        printf("  \033[0;32m✓\033[0m Empty array has length 0\n");
    } else {
        printf("  \033[0;31m✗\033[0m Empty array should have length 0\n");
        xarray_free(empty);
        TEST_FAIL("Empty array test failed");
        return;
    }
    xarray_free(empty);

    TEST_PASS();
}

// ========================================
// Test 6: Memory Safety
// ========================================
static void test_memory_safety(void) {
    TEST_START("Memory Safety Tests");

    SECTION("Test 1: Free empty array");
    XArray *empty_array = XARRAY_NEW(int);
    printf("  Created empty array: %p\n", (void*)empty_array);
    printf("  Length: %u\n", empty_array->length);
    xarray_free(empty_array);
    printf("  Freed empty array successfully\n");

    SECTION("Test 2: Multiple pushes then free");
    XArray *array = XARRAY_NEW(int);
    printf("  Pushing 100 entries...\n");
    for (int i = 0; i < 100; i++) {
        xarray_push(array, &i);
    }
    printf("  Array length after 100 pushes: %u\n", array->length);
    xarray_free(array);
    printf("  Freed populated array successfully\n");

    SECTION("Test 3: Reallocation behavior");
    XArray *grow_array = XARRAY_NEW(int);
    printf("  Growing array from 1 to 1000 elements...\n");
    for (int i = 0; i < 1000; i++) {
        xarray_push(grow_array, &i);
        if (i % 100 == 0) {
            printf("    Size at index %d: %u\n", i, grow_array->length);
        }
    }
    printf("  Final length: %u\n", grow_array->length);

    // Verify all values are still correct after reallocations
    int errors = 0;
    for (int i = 0; i < 1000; i++) {
        const int *val = (int*)xarray_at(grow_array, (unsigned)i);
        if (*val != i) {
            errors++;
        }
    }
    if (errors == 0) {
        printf("  \033[0;32m✓\033[0m All 1000 values verified after reallocations!\n");
    } else {
        printf("  \033[0;31m✗\033[0m Found %d errors after reallocations!\n", errors);
        xarray_free(grow_array);
        TEST_FAIL("Reallocation test had errors");
        return;
    }
    xarray_free(grow_array);

    TEST_PASS();
}

// ========================================
// Test 7: Stress Test (Many Entries)
// ========================================
static void test_stress(void) {
    TEST_START("Stress Test (10000 Entries)");

    XArray *array = XARRAY_NEW(int);

    SECTION("Pushing 10000 entries");
    for (int i = 0; i < 10000; i++) {
        xarray_push(array, &i);
    }
    printf("  Pushed 10000 entries\n");
    printf("  Final array length: %u\n", array->length);

    SECTION("Verifying all entries");
    int errors = 0;
    for (int i = 0; i < 10000; i++) {
        const int *retrieved = (int*)xarray_at(array, (unsigned)i);
        if (*retrieved != i) {
            errors++;
            if (errors <= 5) {
                printf(
                "  \033[0;31m✗\033[0m [%d] expected %d, got %d\n",
                    i, i, *retrieved);
            }
        }
    }

    if (errors == 0) {
        printf("  All 10000 entries verified successfully\n");
        printf("  \033[0;32m✓\033[0m No errors!\n");
        TEST_PASS();
    } else {
        printf("  \033[0;31m✗\033[0m Found %d errors!\n", errors);
        TEST_FAIL("Stress test had errors");
    }

    xarray_free(array);
}
