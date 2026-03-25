extern void run_all_xhashmap_tests(void);
extern void run_all_xarray_tests(void);

int main(void) {
    run_all_xhashmap_tests();
    run_all_xarray_tests();

    return 0;
}
