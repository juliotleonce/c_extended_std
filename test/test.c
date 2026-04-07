extern void run_all_xhashset_tests(void);
extern void run_all_xhashmap_tests(void);
extern void run_all_xarray_tests(void);
extern void run_all_xlinked_list_tests(void);
extern void run_all_xstring_tests(void);

int main(void) {
    run_all_xstring_tests();
    run_all_xlinked_list_tests();
    run_all_xhashset_tests();
    run_all_xhashmap_tests();
    run_all_xarray_tests();

    return 0;
}
