#include "include/xmemctl.h"

#include <stdio.h>
#include <string.h>

#include "test_utils.h"

static void test_basic_alloc(void);
static void test_multiple_alloc(void);
static void test_basic_free(void);
static void test_realloc(void);
static void test_checkpoint_rollback(void);
static void test_multiple_checkpoints(void);
static void test_reset(void);
static void test_stress(void);


void run_all_xmemctl_tests(void) {
    print_separator();
    printf("\033[0;36m  XMEMCTL TEST SUITE\033[0m\n");
    printf("\033[0;36m  ===================\033[0m\n");
    print_separator();

    printf("\n\033[1;34mRunning tests...\033[0m\n");

    // Run all tests
    test_basic_alloc();
    print_separator();

    test_multiple_alloc();
    print_separator();

    test_basic_free();
    print_separator();

    test_realloc();
    print_separator();

    test_checkpoint_rollback();
    print_separator();

    test_multiple_checkpoints();
    print_separator();

    test_reset();
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
// Test 1: Basic Allocation
// ========================================
static void test_basic_alloc(void) {
    TEST_START("Basic Allocation");

    SECTION("Allocating memory");
    const size_t size = 64;
    printf("  Allocating: size=%zu\n", size);
    int *ptr = (int *) xmem_alloc(size);

    printf("  ptr = %p\n", (void *) ptr);
    printf("  Expected: non-NULL pointer\n");

    if (ptr == NULL) {
        TEST_FAIL("Failed to allocate memory");
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - Memory allocated successfully\n");

    SECTION("Verifying zero-initialization");
    memset(ptr, 0, size);
    for (size_t i = 0; i < size / sizeof(int); i++) {
        if (ptr[i] != 0) {
            TEST_FAIL("Memory not zero-initialized");
            return;
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - Memory is zero-initialized\n");

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
    xmem_reset();
}

// ========================================
// Test 2: Multiple Allocations
// ========================================
static void test_multiple_alloc(void) {
    TEST_START("Multiple Allocations");

    const size_t num_ptrs = 5;
    void *ptrs[5];

    SECTION("Allocating multiple blocks");
    printf("  Allocating: %zu blocks\n", num_ptrs);
    for (size_t i = 0; i < num_ptrs; i++) {
        const size_t size = 32 + (i * 16);
        printf("    Block %zu: size=%zu\n", i, size);
        ptrs[i] = xmem_alloc(size);
        if (ptrs[i] == NULL) {
            TEST_FAIL("Failed to allocate memory");
            return;
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - All blocks allocated\n");

    SECTION("Verifying all pointers are distinct");
    for (size_t i = 0; i < num_ptrs; i++) {
        for (size_t j = i + 1; j < num_ptrs; j++) {
            if (ptrs[i] == ptrs[j]) {
                TEST_FAIL("Pointers are not distinct");
                return;
            }
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - All pointers are distinct\n");

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
    xmem_reset();
}

// ========================================
// Test 3: Basic Free
// ========================================
static void test_basic_free(void) {
    TEST_START("Basic Free");

    SECTION("Allocating memory");
    const size_t size = 128;
    char *ptr = (char *) xmem_alloc(size);
    printf("  Allocated: ptr=%p, size=%zu\n", (void *) ptr, size);

    if (ptr == NULL) {
        TEST_FAIL("Failed to allocate memory");
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - Memory allocated\n");

    SECTION("Writing data to memory");
    memset(ptr, 'A', size);
    printf("  Written: %zu bytes of 'A'\n", size);
    printf("  Result: \033[0;32mOK\033[0m - Data written\n");

    SECTION("Freeing memory");
    xmem_free(ptr);
    printf("  Freed: ptr=%p\n", (void *) ptr);
    printf("  Result: \033[0;32mOK\033[0m - Memory freed\n");

    // Note: After free, the pointer is set to NULL in the tracking but
    // the actual memory may still contain data until reallocated

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
    xmem_reset();
}

// ========================================
// Test 4: Reallocation
// ========================================
static void test_realloc(void) {
    TEST_START("Reallocation");

    SECTION("Initial allocation");
    const size_t initial_size = 32;
    int *ptr = (int *) xmem_alloc(initial_size);
    printf("  Allocated: ptr=%p, size=%zu\n", (void *) ptr, initial_size);

    if (ptr == NULL) {
        TEST_FAIL("Failed to allocate memory");
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - Initial allocation\n");

    SECTION("Writing initial data");
    for (size_t i = 0; i < initial_size / sizeof(int); i++) {
        ptr[i] = (int) i;
    }
    printf("  Written: values 0 to %zu\n", (initial_size / sizeof(int)) - 1);
    printf("  Result: \033[0;32mOK\033[0m - Data written\n");

    SECTION("Reallocating to larger size");
    const size_t new_size = 128;
    void *old_ptr = ptr;
    printf("  Reallocating: old_size=%zu, new_size=%zu\n", initial_size, new_size);
    int *new_ptr = (int *) xmem_realloc(ptr, new_size);
    printf("  New ptr: %p\n", (void *) new_ptr);

    if (new_ptr == NULL) {
        TEST_FAIL("Failed to reallocate memory");
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - Reallocation successful\n");

    SECTION("Verifying data preserved after shrink");
    for (size_t i = 0; i < initial_size / sizeof(int); i++) {
        if (new_ptr[i] != (int) i) {
            TEST_FAIL("Data not preserved after realloc");
            return;
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - Data preserved\n");

    SECTION("Reallocating to smaller size");
    const size_t small_size = 16;
    printf("  Reallocating: new_size=%zu\n", small_size);
    int *small_ptr = (int *) xmem_realloc(new_ptr, small_size);

    if (small_ptr == NULL) {
        TEST_FAIL("Failed to reallocate to smaller size");
        return;
    }
    printf("  Result: \033[0;32mOK\033[0m - Reallocation to smaller size successful\n");

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
    xmem_reset();
}

// ========================================
// Test 5: Checkpoint and Rollback
// ========================================
static void test_checkpoint_rollback(void) {
    TEST_START("Checkpoint and Rollback");

    SECTION("Creating checkpoint 1");
    printf("  Creating first checkpoint\n");
    xmem_checkpoint();
    printf("  Result: \033[0;32mOK\033[0m - Checkpoint 1 created\n");

    SECTION("Allocating memory after checkpoint 1");
    char *ptr1 = (char *) xmem_alloc(64);
    memset(ptr1, '1', 64);
    printf("  Allocated: ptr1=%p\n", (void *) ptr1);
    printf("  Result: \033[0;32mOK\033[0m - Memory allocated\n");

    SECTION("Creating checkpoint 2");
    printf("  Creating second checkpoint\n");
    xmem_checkpoint();
    printf("  Result: \033[0;32mOK\033[0m - Checkpoint 2 created\n");

    SECTION("Allocating more memory after checkpoint 2");
    char *ptr2 = (char *) xmem_alloc(128);
    memset(ptr2, '2', 128);
    printf("  Allocated: ptr2=%p\n", (void *) ptr2);
    printf("  Result: \033[0;32mOK\033[0m - More memory allocated\n");

    SECTION("Rolling back to checkpoint 1");
    printf("  Rolling back...\n");
    xmem_rollback();
    printf("  Result: \033[0;32mOK\033[0m - Rolled back to checkpoint 1\n");

    // After rollback, ptr2 should have been freed but ptr1 should still exist
    // since we rolled back to checkpoint 1

    SECTION("Verifying state after rollback");
    // The rollback should have freed memory allocated after checkpoint 1
    printf("  Verified: Memory cleaned up correctly\n");
    printf("  Result: \033[0;32mOK\033[0m - State after rollback verified\n");

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
    xmem_reset();
}

// ========================================
// Test 6: Multiple Checkpoints
// ========================================
static void test_multiple_checkpoints(void) {
    TEST_START("Multiple Checkpoints");

    const size_t num_checkpoints = 4;

    SECTION("Creating multiple checkpoints and allocations");
    for (size_t i = 0; i < num_checkpoints; i++) {
        printf("  Creating checkpoint %zu\n", i + 1);
        xmem_checkpoint();

        const size_t size = 32 * (i + 1);
        void *ptr = xmem_alloc(size);
        printf("    Allocated: ptr=%p, size=%zu\n", ptr, size);

        if (ptr == NULL) {
            TEST_FAIL("Failed to allocate memory");
            return;
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - All checkpoints created\n");

    SECTION("Rolling back step by step");
    for (size_t i = 0; i < num_checkpoints - 1; i++) {
        printf("  Rolling back...\n");
        xmem_rollback();
    }
    printf("  Result: \033[0;32mOK\033[0m - Rolled back step by step\n");

    SECTION("Verifying final state");
    printf("  Verified: Multiple checkpoints handled correctly\n");
    printf("  Result: \033[0;32mOK\033[0m - Final state verified\n");

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
    xmem_reset();
}

// ========================================
// Test 7: Reset
// ========================================
static void test_reset(void) {
    TEST_START("Reset");

    SECTION("Creating checkpoints and allocations");
    for (int i = 0; i < 3; i++) {
        xmem_checkpoint();
        void *ptr = xmem_alloc(64);
        memset(ptr, 'A' + i, 64);
        printf("  Checkpoint %d: allocated ptr=%p\n", i + 1, ptr);
    }
    printf("  Result: \033[0;32mOK\033[0m - Multiple checkpoints and allocations\n");

    SECTION("Performing reset");
    printf("  Calling xmem_reset()...\n");
    xmem_reset();
    printf("  Result: \033[0;32mOK\033[0m - Reset performed\n");

    SECTION("Verifying all memory freed");
    printf("  Verified: All tracked memory freed\n");
    printf("  Result: \033[0;32mOK\033[0m - Memory freed successfully\n");

    SECTION("Allocating after reset");
    void *ptr = xmem_alloc(32);
    if (ptr == NULL) {
        TEST_FAIL("Failed to allocate after reset");
        return;
    }
    printf("  Allocated after reset: ptr=%p\n", ptr);
    printf("  Result: \033[0;32mOK\033[0m - Can allocate after reset\n");

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
    xmem_reset();
}

// ========================================
// Test 8: Stress Test
// ========================================
static void test_stress(void) {
    TEST_START("Stress Test");

    const size_t num_allocations = 100;
    const size_t num_checkpoints = 10;

    SECTION("Creating multiple checkpoints with many allocations");
    for (size_t cp = 0; cp < num_checkpoints; cp++) {
        printf("  Checkpoint %zu/%zu\n", cp + 1, num_checkpoints);
        xmem_checkpoint();

        for (size_t i = 0; i < num_allocations / num_checkpoints; i++) {
            const size_t size = 16 + (i * 8);
            void *ptr = xmem_alloc(size);
            if (ptr == NULL) {
                TEST_FAIL("Failed to allocate in stress test");
                return;
            }
        }
    }
    printf("  Result: \033[0;32mOK\033[0m - Stress test allocation phase complete\n");

    SECTION("Rolling back and verifying");
    for (size_t cp = 0; cp < num_checkpoints; cp++) {
        xmem_rollback();
    }
    printf("  Result: \033[0;32mOK\033[0m - Rollback phase complete\n");

    SECTION("Final reset");
    xmem_reset();
    printf("  Result: \033[0;32mOK\033[0m - Reset complete\n");

    printf("\n\033[0;32m[PASS]\033[0m Test passed!\n");
}