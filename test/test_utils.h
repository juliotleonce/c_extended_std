#ifndef EXTENDED_STD_TEST_MACROS_H
#define EXTENDED_STD_TEST_MACROS_H

#include <stdio.h>
#include <stdlib.h>

#define TEST_START(name) do { \
printf("\n"); \
printf("========================================\n"); \
printf("  TEST: %s\n", name); \
printf("========================================\n"); \
} while(0)

#define TEST_PASS() do { \
printf("\033[0;32m[PASS]\033[0m Test passed!\n"); \
} while(0)

#define TEST_FAIL(msg) do { \
printf("\033[0;31m[FAIL]\033[0m %s\n", msg); \
exit(0); \
} while(0)

#define SECTION(msg) do { \
printf("\n--- %s ---\n", msg); \
} while(0)

// Visual separator
void print_separator(void);

#endif //EXTENDED_STD_TEST_MACROS_H