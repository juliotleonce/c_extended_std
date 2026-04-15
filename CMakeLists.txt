cmake_minimum_required(VERSION 4.2)
project(xstdlib LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "Installation prefix" FORCE)
endif()

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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(DIRECTORY include/ DESTINATION include/xstdlib)

install(TARGETS xstdlib
        EXPORT xstdlibTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(EXPORT xstdlibTargets
        FILE xstdlibTargets.cmake
        NAMESPACE xstdlib::
        DESTINATION lib/cmake/xstdlib
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/xstdlibConfig.cmake"
        "include(\${CMAKE_CURRENT_LIST_DIR}/xstdlibTargets.cmake)\n")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/xstdlibConfig.cmake"
        DESTINATION lib/cmake/xstdlib)

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