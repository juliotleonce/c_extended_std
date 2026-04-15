cmake_minimum_required(VERSION 4.2)
project(xstdlib LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)

# Set installation prefix to ~/.local by default
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "Installation prefix" FORCE)
endif()

# Build as shared library by default
set(BUILD_SHARED_LIBS ON)

add_library(xstdlib SHARED
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
        include/xmemctl.h
        lib-src/xmemctl.c
        include/xstdlib.h
)

target_include_directories(xstdlib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set version info for the library
set_target_properties(xstdlib PROPERTIES
    VERSION "1.0.0"
    SOVERSION 1
)

# Install the library
install(TARGETS xstdlib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
)

# Install headers
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Create CMake config file for find_package
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/xstdlibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/xstdlibConfig.cmake
    INSTALL_DESTINATION lib/cmake/xstdlib
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/xstdlibConfig.cmake
    DESTINATION lib/cmake/xstdlib
)

add_executable(test_xstdlib test/test.c
        test/xstring_test.c
        test/xhashmap_test.c
        test/xhashset_test.c
        test/xarray_test.c
        test/xlinked_list_test.c
        test/xmemctl_test.c
        test/test_utils.h
        test/test_utils.c
        include/xstdlib.h)

target_link_libraries(test_xstdlib PRIVATE xstdlib)