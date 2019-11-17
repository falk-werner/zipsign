cmake_minimum_required (VERSION 3.1)
project(zipsign VERSION 1.0.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)


set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(zipsign STATIC)

target_sources(zipsign
    PRIVATE
        src/zipsign/lazy.c
        src/zipsign/status.c
        src/zipsign/impl/lazy.c
        src/zipsign/impl/status.c
)
target_include_directories(zipsign PRIVATE include src)
target_compile_options(zipsign PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(zipsign PUBLIC ${OPENSSL_LIBRARIES})

add_executable(cli
    src/cli/main.c)
set_target_properties(cli PROPERTIES OUTPUT_NAME "zipsign")
target_include_directories(cli PRIVATE include src/cli)
target_link_libraries(cli PUBLIC zipsign)