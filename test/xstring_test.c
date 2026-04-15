#include "../include/xstring.h"
#include "test_utils.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "xmemctl.h"

void test_xstring_new_free() {
    TEST_START("xstring_new_free");
    
    SECTION("Basic creation");
    XString *s = xstring_new("Hello");
    assert(s->length == 5);
    assert(strcmp(s->c_str, "Hello") == 0);
    xstring_free(s);

    SECTION("Empty string creation");
    XString *empty = xstring_new("");
    assert(empty->length == 0);
    assert(strcmp(empty->c_str, "") == 0);
    xstring_free(empty);

    SECTION("Large string creation");
    const char *large_str = "This is a very long string used to test if memory allocation and strlen works fine with large inputs. Let's make it even longer to be sure it's not a trivial case.";
    XString *large = xstring_new(large_str);
    assert(large->length == strlen(large_str));
    assert(strcmp(large->c_str, large_str) == 0);
    xstring_free(large);

    TEST_PASS();
}

void test_xstring_copy() {
    TEST_START("xstring_copy");
    
    XString *s1 = xstring_new("Original");
    XString *s2 = xstring_copy(s1);
    
    assert(s1->length == s2->length);
    assert(strcmp(s1->c_str, s2->c_str) == 0);
    assert(s1->c_str != s2->c_str); // Must be different memory
    
    xstring_free(s1);
    xstring_free(s2);
    
    TEST_PASS();
}

void test_xstring_concat() {
    TEST_START("xstring_concat");
    
    SECTION("Standard concatenation");
    XString *s1 = xstring_new("Hello ");
    XString *s2 = xstring_new("World!");
    XString *res = xstring_concat(s1, s2);
    assert(res->length == 12);
    assert(strcmp(res->c_str, "Hello World!") == 0);
    xstring_free(s1);
    xstring_free(s2);
    xstring_free(res);

    SECTION("Concatenation with empty string");
    XString *empty = xstring_new("");
    XString *content = xstring_new("Content");
    XString *res1 = xstring_concat(empty, content);
    XString *res2 = xstring_concat(content, empty);
    assert(strcmp(res1->c_str, "Content") == 0);
    assert(strcmp(res2->c_str, "Content") == 0);
    xstring_free(empty);
    xstring_free(content);
    xstring_free(res1);
    xstring_free(res2);

    SECTION("Concatenation with C-string");
    XString *base = xstring_new("Base");
    XString *res3 = xstring_concat_c_str(base, "Suffix");
    assert(strcmp(res3->c_str, "BaseSuffix") == 0);
    xstring_free(base);
    xstring_free(res3);

    TEST_PASS();
}

void test_xstring_substring() {
    TEST_START("xstring_substring");
    
    XString *s = xstring_new("0123456789");
    
    SECTION("Middle substring");
    XString *sub1 = xstring_substring(s, 2, 5); // "234"
    assert(sub1->length == 3);
    assert(strcmp(sub1->c_str, "234") == 0);
    xstring_free(sub1);
    
    SECTION("Start to middle");
    XString *sub2 = xstring_substring(s, 0, 5); // "01234"
    assert(sub2->length == 5);
    assert(strcmp(sub2->c_str, "01234") == 0);
    xstring_free(sub2);
    
    SECTION("Middle to end");
    XString *sub3 = xstring_substring(s, 5, 10); // "56789"
    assert(sub3->length == 5);
    assert(strcmp(sub3->c_str, "56789") == 0);
    xstring_free(sub3);
    
    SECTION("Empty substring (same indices)");
    XString *sub4 = xstring_substring(s, 3, 3);
    assert(sub4->length == 0);
    assert(strcmp(sub4->c_str, "") == 0);
    xstring_free(sub4);
    
    xstring_free(s);
    TEST_PASS();
}

void test_xstring_find_index() {
    TEST_START("xstring_find_index");
    
    XString *s = xstring_new("The quick brown fox jumps over the lazy dog");
    
    SECTION("Finding first word");
    assert(xstring_find_first_index_of(s, "The") == 0);
    
    SECTION("Finding middle word");
    assert(xstring_find_first_index_of(s, "fox") == 16);
    
    SECTION("Finding non-existent");
    assert(xstring_find_first_index_of(s, "cat") == -1);
    
    SECTION("Finding with partial matches");
    XString *s2 = xstring_new("ababcabcac");
    assert(xstring_find_first_index_of(s2, "abc") == 2);
    xstring_free(s2);

    SECTION("Finding empty substring");
    assert(xstring_find_first_index_of(s, "") == -1); // Usually 0 or some consistent value

    xstring_free(s);
    TEST_PASS();
}

void test_xstring_replace() {
    TEST_START("xstring_replace");
    
    SECTION("Simple replace");
    XString *s = xstring_new("Hello world");
    XString *res = xstring_replace(s, "world", "Junie");
    assert(strcmp(res->c_str, "Hello Junie") == 0);
    xstring_free(s);
    xstring_free(res);
    
    SECTION("Replace with larger string");
    XString *s2 = xstring_new("A B C");
    XString *res2 = xstring_replace(s2, "B", "Banana");
    assert(strcmp(res2->c_str, "A Banana C") == 0);
    xstring_free(s2);
    xstring_free(res2);
    
    SECTION("Replace with smaller string");
    XString *s3 = xstring_new("A BIG APPLE");
    XString *res3 = xstring_replace(s3, "BIG ", "");
    assert(strcmp(res3->c_str, "A APPLE") == 0);
    xstring_free(s3);
    xstring_free(res3);
    
    SECTION("Non-existent replacement");
    XString *s4 = xstring_new("Keep it simple");
    XString *res4 = xstring_replace(s4, "hard", "easy");
    assert(strcmp(res4->c_str, "Keep it simple") == 0);
    xstring_free(s4);
    xstring_free(res4);

    TEST_PASS();
}

void test_xstring_case_conversions() {
    TEST_START("xstring_case_conversions");
    
    XString *s = xstring_new("MixEd CASe 123!");
    
    SECTION("To Upper");
    XString *upper = xstring_to_upper(s);
    assert(strcmp(upper->c_str, "MIXED CASE 123!") == 0);
    xstring_free(upper);
    
    SECTION("To Lower");
    XString *lower = xstring_to_lower(s);
    assert(strcmp(lower->c_str, "mixed case 123!") == 0);
    xstring_free(lower);
    
    xstring_free(s);
    TEST_PASS();
}

void test_xstring_predicates() {
    TEST_START("xstring_predicates");
    
    XString *s1 = xstring_new("Comparison");
    XString *s2 = xstring_new("Comparison");
    XString *s3 = xstring_new("different");
    
    SECTION("Equality");
    assert(xstring_equal(s1, s2) == true);
    assert(xstring_equal(s1, s3) == false);
    assert(xstring_equal_c_str(s1, "Comparison") == true);
    assert(xstring_equal_c_str(s1, "COMPARE") == false);
    
    SECTION("Contains");
    assert(xstring_contains(s1, "par") == true);
    assert(xstring_contains(s1, "xxx") == false);
    
    xstring_free(s1);
    xstring_free(s2);
    xstring_free(s3);
    TEST_PASS();
}

void test_xstring_split() {
    TEST_START("xstring_split");

    SECTION("Split with space");
    XMEM_SCOPE {
        XString *s1 = xstring_new("Hello World Junie");
        XArray *res1 = xstring_split(s1, " ");
        // Doit contenir "Hello", "World", "Junie"
        // Note: L'implémentation actuelle pourrait échouer ici si elle ne gère pas le dernier segment.
        assert(res1->length == 3);
        assert(xstring_equal_c_str(xarray_at(res1, 0), "Hello"));
        assert(xstring_equal_c_str(xarray_at(res1, 1), "World"));
        assert(xstring_equal_c_str(xarray_at(res1, 2), "Junie"));
    }

    SECTION("Split with comma");
    XMEM_SCOPE {
        XString *s2 = xstring_new("apple,banana,cherry");
        XArray *res2 = xstring_split(s2, ",");
        assert(res2->length == 3);
        assert(xstring_equal_c_str(xarray_at(res2, 0), "apple"));
        assert(xstring_equal_c_str(xarray_at(res2, 1), "banana"));
        assert(xstring_equal_c_str(xarray_at(res2, 2), "cherry"));
    }


    SECTION("No separator found");
    XMEM_SCOPE {
        XString *s3 = xstring_new("nosplit");
        XArray *res3 = xstring_split(s3, " ");
        assert(res3->length == 1);
        assert(xstring_equal_c_str(xarray_at(res3, 0), "nosplit"));
    }
    TEST_PASS();
}

void run_all_xstring_tests() {
    printf("\n>>> RUNNING XSTRING TESTS <<<\n");
    test_xstring_new_free();
    test_xstring_copy();
    test_xstring_concat();
    test_xstring_substring();
    test_xstring_find_index();
    test_xstring_replace();
    test_xstring_split();
    test_xstring_case_conversions();
    test_xstring_predicates();
}
