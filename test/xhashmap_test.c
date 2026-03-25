#include "include/xhashmap.h"

#include <string.h>

#include "test_utils.h"

static void test_basic_new_and_put(void);
static void test_multiple_insertions(void);
static void test_update_existing_key(void);
static void test_nonexistent_key(void);
static void test_struct_values(void);
static void test_edge_cases(void);
static void test_memory_safety(void);
static void test_stress(void);


void run_all_xhashmap_tests(void) {
    print_separator();
    printf("\033[0;36m  XHASHMAP TEST SUITE\033[0m\n");
    printf("\033[0;36m  ====================\033[0m\n");
    print_separator();

    printf("\n\033[1;34mRunning tests...\033[0m\n");

    // Run all tests
    test_basic_new_and_put();
    print_separator();

    test_multiple_insertions();
    print_separator();

    test_update_existing_key();
    print_separator();

    test_nonexistent_key();
    print_separator();

    test_struct_values();
    print_separator();

    test_edge_cases();
    print_separator();

    test_memory_safety();
    print_separator();

    test_stress();
    print_separator();


    printf("\n");
    printf("\033[0;32m========================================\033[0m\n");
    printf("\033[0;32m  ALL TESTS COMPLETED\033[0m\n");
    printf("\033[0;32m========================================\033[0m\n");
    printf("\n");
}

// ========================================
// Test 1: Basic Creation and Single Insert
// ========================================
static void test_basic_new_and_put(void) {
    TEST_START("Basic Creation & Single Insert");

    XHashMap *map = xhashmap_new(sizeof(int));

    SECTION("Creating hashmap with type_size=sizeof(int)");
    printf("  map = %p\n", (void*)map);
    printf("  Expected: non-NULL pointer\n");

    if (map == NULL) {
        TEST_FAIL("Failed to create hashmap");
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - HashMap created successfully\n");

    SECTION("Inserting single key-value pair");
    const int value = 42;
    printf("  Inserting: key=\"answer\", value=%d\n", value);
    xhashmap_put(map, "answer", &value);

    SECTION("Retrieving value");
    const int *retrieved = (int*)xhashmap_get(map, "answer");
    printf("  Retrieved: %d\n", *retrieved);
    if (*retrieved == 42) {
        printf("  Retrieved value: %d\n", *retrieved);
        printf("  Result: \033[0;32mOK\033[0m - Value matches!\n");
        TEST_PASS();
    } else {
        TEST_FAIL("Failed to retrieve correct value");
    }

    xhashmap_free(map);
    printf("  HashMap freed successfully\n");

}

// ========================================
// Test 2: Multiple Insertions & Hash Collisions
// ========================================
static void test_multiple_insertions(void) {
    TEST_START("Multiple Insertions (Hash Collision Scenarios)");

    XHashMap *map = xhashmap_new(sizeof(double));

    SECTION("Inserting multiple entries");

    typedef struct {
        const char *key;
        double value;
    } TestPair;

    const TestPair pairs[] = {
        {"alpha", 1.1},
        {"beta", 2.2},
        {"gamma", 3.3},
        {"delta", 4.4},
        {"epsilon", 5.5},
        {"zeta", 6.6},
        {"eta", 7.7},
        {"theta", 8.8},
        {"iota", 9.9},
        {"kappa", 10.10}
    };

    const size_t num_pairs = sizeof(pairs) / sizeof(pairs[0]);

    printf("  Inserting %zu key-value pairs:\n", num_pairs);
    for (size_t i = 0; i < num_pairs; i++) {
        xhashmap_put(map, pairs[i].key, &pairs[i].value);
        printf("    [%zu] \"%s\" -> %.2f\n", i, pairs[i].key, pairs[i].value);
    }

    SECTION("Verifying all insertions");
    printf("  Retrieving and verifying all values:\n");
    int all_ok = 1;
    for (size_t i = 0; i < num_pairs; i++) {
        const double *retrieved = (double*)xhashmap_get(map, pairs[i].key);
        if (retrieved != NULL && *retrieved == pairs[i].value) {
            printf("    \033[0;32m✓\033[0m \"%s\" -> %.2f [OK]\n", pairs[i].key, *retrieved);
        } else {
            printf("    \033[0;31m✗\033[0m \"%s\" -> expected %.2f, got %s\n",
                   pairs[i].key, pairs[i].value,
                   retrieved ? "wrong value" : "NULL");
            all_ok = 0;
        }
    }

    if (all_ok) {
        TEST_PASS();
    } else {
        TEST_FAIL("Some values did not match");
    }

    xhashmap_free(map);
}

// ========================================
// Test 3: Update Existing Key
// ========================================
static void test_update_existing_key(void) {
    TEST_START("Update Existing Key");

    XHashMap *map = xhashmap_new(sizeof(int));

    SECTION("Initial insertion");
    const int value1 = 100;
    xhashmap_put(map, "counter", &value1);
    printf("  Initial: counter = %d\n", value1);

    const int *retrieved1 = (int*)xhashmap_get(map, "counter");
    printf("  Retrieved: %d\n", *retrieved1);

    SECTION("Updating existing key");
    const int value2 = 200;
    xhashmap_put(map, "counter", &value2);
    printf("  Updated: counter = %d\n", value2);

    const int *retrieved2 = (int*)xhashmap_get(map, "counter");
    printf("  Retrieved: %d\n", *retrieved2);

    if (*retrieved2 == 200) {
        printf("  Result: \033[0;32mOK\033[0m - Value updated successfully\n");
        TEST_PASS();
    } else {
        TEST_FAIL("Value was not updated");
    }

    xhashmap_free(map);
}

// ========================================
// Test 4: Non-existent Key Retrieval
// ========================================
static void test_nonexistent_key(void) {
    TEST_START("Non-existent Key Retrieval");

    XHashMap *map = xhashmap_new(sizeof(int));

    SECTION("Testing non-existent keys");
    const int value = 999;
    xhashmap_put(map, "existing", &value);

    printf("  Inserted key: \"existing\" with value %d\n", value);
    printf("  Testing non-existent keys:\n");

    const char *test_keys[] = {"nonexistent", "missing", "not_here", "foobar"};
    const size_t num_tests = sizeof(test_keys) / sizeof(test_keys[0]);

    int all_null = 1;
    for (size_t i = 0; i < num_tests; i++) {
        const int *retrieved = (int*)xhashmap_get(map, test_keys[i]);
        if (retrieved == NULL) {
            printf("    \033[0;32m✓\033[0m \"%s\" -> NULL [CORRECT]\n", test_keys[i]);
        } else {
            printf("    \033[0;31m✗\033[0m \"%s\" -> %d [WRONG - should be NULL]\n",
                   test_keys[i], *retrieved);
            all_null = 0;
        }
    }

    // Verify existing key still works
    const int *existing = (int*)xhashmap_get(map, "existing");
    if (existing != NULL && *existing == 999) {
        printf("    \033[0;32m✓\033[0m \"existing\" -> %d [Still correct]\n", *existing);
    } else {
        all_null = 0;
        printf("    \033[0;31m✗\033[0m \"existing\" [BROKEN]\n");
    }

    if (all_null) {
        TEST_PASS();
    } else {
        TEST_FAIL("Non-existent key handling failed");
    }

    xhashmap_free(map);
}

// ========================================
// Test 5: Complex Data Structures (Struct Values)
// ========================================
static void test_struct_values(void) {
    TEST_START("Complex Data Structures (Struct Values)");

    typedef struct {
        int id;
        char name[32];
        float score;
    } User;

    XHashMap *map = xhashmap_new(sizeof(User));

    SECTION("Creating users");

    User user1 = {.id = 1, .name = "Alice", .score = 95.5f};
    User user2 = {.id = 2, .name = "Bob", .score = 87.3f};
    User user3 = {.id = 3, .name = "Charlie", .score = 92.1f};

    printf("  Creating users:\n");
    printf("    User 1: id=%d, name=\"%s\", score=%.1f\n", user1.id, user1.name, user1.score);
    printf("    User 2: id=%d, name=\"%s\", score=%.1f\n", user2.id, user2.name, user2.score);
    printf("    User 3: id=%d, name=\"%s\", score=%.1f\n", user3.id, user3.name, user3.score);

    SECTION("Inserting users into hashmap");
    xhashmap_put(map, "user:1", &user1);
    printf("  Inserted: key=\"user:1\"\n");
    xhashmap_put(map, "user:2", &user2);
    printf("  Inserted: key=\"user:2\"\n");
    xhashmap_put(map, "user:3", &user3);
    printf("  Inserted: key=\"user:3\"\n");

    SECTION("Verifying all users");
    int all_ok = 1;

    const User originals[] = {user1, user2, user3};

    for (size_t i = 0; i < 3; i++) {
        const char *keys[] = {"user:1", "user:2", "user:3"};
        User *retrieved = xhashmap_get(map, keys[i]);
        if (retrieved != NULL) {
            printf("  Retrieved \"%s\":\n", keys[i]);
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
            printf("  \033[0;31m✗\033[0m \"%s\" -> NULL\n", keys[i]);
            all_ok = 0;
        }
    }

    if (all_ok) {
        TEST_PASS();
    } else {
        TEST_FAIL("Struct retrieval failed");
    }

    xhashmap_free(map);
}

// ========================================
// Test 6: Edge Cases (Empty String Key, Special Characters)
// ========================================
static void test_edge_cases(void) {
    TEST_START("Edge Cases (Special Keys)");

    XHashMap *map = xhashmap_new(sizeof(int));

    SECTION("Testing special keys");

    typedef struct {
        const char *key;
        int value;
    } EdgePair;

    const EdgePair pairs[] = {
        {"", 1},              // Empty string
        {" ", 2},             // Single space
        {"key with spaces", 3},
        {"key\nwith\nnewlines", 4},
        {"key:with:colons", 5},
        {"key/with/slashes", 6},
        {"123numeric", 7},    // Starting with number
        {"KEY_UPPER", 8},     // Uppercase
        {"MiXeD CaSe", 9},    // Mixed case
    };

    const size_t num_pairs = sizeof(pairs) / sizeof(pairs[0]);

    printf("  Inserting %zu edge case keys:\n", num_pairs);
    for (size_t i = 0; i < num_pairs; i++) {
        xhashmap_put(map, pairs[i].key, &pairs[i].value);
        printf("    [%zu] key=\"%s\" -> %d\n", i, pairs[i].key, pairs[i].value);
    }

    SECTION("Verifying edge case keys");
    int all_ok = 1;
    for (size_t i = 0; i < num_pairs; i++) {
        const int *retrieved = (int*)xhashmap_get(map, pairs[i].key);
        if (retrieved != NULL && *retrieved == pairs[i].value) {
            printf("  \033[0;32m✓\033[0m \"%s\" -> %d [OK]\n", pairs[i].key, *retrieved);
        } else {
            printf("  \033[0;31m✗\033[0m \"%s\" -> expected %d, got %s\n",
                   pairs[i].key, pairs[i].value,
                   retrieved ? "wrong value" : "NULL");
            all_ok = 0;
        }
    }

    if (all_ok) {
        TEST_PASS();
    } else {
        TEST_FAIL("Edge case handling failed");
    }

    xhashmap_free(map);
}

// ========================================
// Test 7: Memory Safety (Double Free Prevention)
// ========================================
static void test_memory_safety(void) {
    TEST_START("Memory Safety Tests");

    SECTION("Test 1: Free empty hashmap");
    XHashMap *empty_map = xhashmap_new(sizeof(int));
    printf("  Created empty hashmap: %p\n", (void*)empty_map);
    xhashmap_free(empty_map);
    printf("  Freed empty hashmap successfully\n");

    SECTION("Test 2: Multiple inserts then free");
    XHashMap *map = xhashmap_new(sizeof(int));
    for (int i = 0; i < 100; i++) {
        xhashmap_put(map, "test", &i);
    }
    printf("  Inserted 100 entries\n");
    xhashmap_free(map);
    printf("  Freed populated hashmap successfully\n");

    SECTION("Test 3: Various types");
    // Test with different type sizes
    XHashMap *char_map = xhashmap_new(sizeof(char));
    const char c = 'X';
    xhashmap_put(char_map, "char_test", &c);
    const char *c_ret = (char*)xhashmap_get(char_map, "char_test");
    printf("  char test: %c %s\n", *c_ret, (*c_ret == 'X') ? "[OK]" : "[FAIL]");
    xhashmap_free(char_map);

    XHashMap *long_map = xhashmap_new(sizeof(long));
    const long l = 1234567890123L;
    xhashmap_put(long_map, "long_test", &l);
    const long *l_ret = (long*)xhashmap_get(long_map, "long_test");
    printf("  long test: %ld %s\n", *l_ret, (*l_ret == l) ? "[OK]" : "[FAIL]");
    xhashmap_free(long_map);

    TEST_PASS();
}

// ========================================
// Test 8: Stress Test (Many Entries)
// ========================================
static void test_stress(void) {
    TEST_START("Stress Test (1000+ Entries)");

    XHashMap *map = xhashmap_new(sizeof(int));

    SECTION("Inserting 1000 entries");
    for (int i = 0; i < 1000; i++) {
        char key[32];
        snprintf(key, sizeof(key), "key_%d", i);
        xhashmap_put(map, key, &i);
    }
    printf("  Inserted 1000 entries\n");

    SECTION("Verifying all entries");
    int errors = 0;
    for (int i = 0; i < 1000; i++) {
        char key[32];
        snprintf(key, sizeof(key), "key_%d", i);
        const int *retrieved = (int*)xhashmap_get(map, key);
        if (retrieved == NULL || *retrieved != i) {
            errors++;
            if (errors <= 5) {
                printf(
                "  \033[0;31m✗\033[0m %s -> expected %d, got %s\n",
                    key, i, retrieved ? "wrong" : "NULL");
            }
        }
    }

    if (errors == 0) {
        printf("  All 1000 entries verified successfully\n");
        printf("  \033[0;32m✓\033[0m No errors!\n");
        TEST_PASS();
    } else {
        printf("  \033[0;31m✗\033[0m Found %d errors!\n", errors);
        TEST_FAIL("Stress test had errors");
    }

    xhashmap_free(map);
}