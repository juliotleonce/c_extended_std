cmake_minimum_required(VERSION 4.2)
project(extended_std C)

set(CMAKE_C_STANDARD 11)

add_library(extended_std SHARED
        lib-src/xarray.c
        include/xarray.h
        lib-src/xhashmap.c
        include/xhashmap.h
        include/xhashset.h
        lib-src/xhashset.c
        include/xstring.h
        lib-src/xstring.c
        include/xlinked_list.h
        lib-src/xlinked_list.c
)

target_include_directories(extended_std PUBLIC .)

add_executable(test_custom_std test/test.c
        test/xstring_test.c
        test/xhashmap_test.c
        test/xhashset_test.c
        test/xarray_test.c
        test/xlinked_list_test.c
        test/test_utils.h
        test/test_utils.c)

target_link_libraries(test_custom_std PRIVATE extended_std)