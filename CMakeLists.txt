cmake_minimum_required (VERSION 3.1)
project(zipsign VERSION 1.0.0)

option(WITHOUT_TESTS   "disable unit tests"   OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)


set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(zipsign STATIC
        src/zipsign/lazy.c
        src/zipsign/impl/lazy.c
        src/zipsign/error_code.c
        src/zipsign/impl/error_code.c
        src/zipsign/error.c
        src/zipsign/impl/error.c
        src/zipsign/openssl.c
        src/zipsign/impl/openssl.c
        src/zipsign/impl/zip/zip.c
        src/zipsign/impl/cms/cms.c
)
target_include_directories(zipsign PRIVATE include src)
target_compile_options(zipsign PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(zipsign PUBLIC ${OPENSSL_LIBRARIES})

add_executable(cli
    src/cli/main.c)
set_target_properties(cli PROPERTIES OUTPUT_NAME "zipsign")
target_include_directories(cli PRIVATE include src/cli)
target_link_libraries(cli PRIVATE zipsign)

# Unit tests

if(NOT WITHOUT_TESTS)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include (CTest)

pkg_check_modules(GTEST gtest_main)
include(GoogleTest)
pkg_check_modules(GMOCK gmock)

add_executable(alltests
    test/error.cc
)

target_include_directories(alltests PRIVATE
    include
    src
    ${GMOCK_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(alltests PRIVATE zipsign)
target_compile_options(alltests PRIVATE ${GMOCK_CFLAGS} ${GTEST_CFLAGS} "-pthread")
target_link_libraries(alltests PRIVATE zipsign ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()
gtest_discover_tests(alltests TEST_PREFIX alltests:)

add_custom_target(check
    ./alltests)
add_dependencies(check alltests)

endif(NOT WITHOUT_TESTS)
