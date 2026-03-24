cmake_minimum_required(VERSION 4.2)
project(extended_std C)

set(CMAKE_C_STANDARD 11)

add_library(extended_std SHARED
        xarray.c
        xarray.h)

target_include_directories(extended_std PUBLIC .)

add_executable(test_custom_std test.c)

target_link_libraries(test_custom_std PRIVATE extended_std)