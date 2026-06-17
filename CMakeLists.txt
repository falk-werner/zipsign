cmake_minimum_required (VERSION 3.16)
project(zipsign 
    VERSION 1.7.0
    DESCRIPTION "Sign and verify ZIP archives."
    HOMEPAGE_URL "https://github.com/falk-werner/zipsign"
    LANGUAGES CXX
)

option(WITHOUT_TESTS   "disable unit tests"   OFF)
option(WITHOUT_DEB     "disable cpack configuration for debian packages" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl>=3.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(coverage)

add_library(zipsign STATIC
    lib/openssl++/exception.cpp
    lib/openssl++/basic_io.cpp
    lib/openssl++/private_key.cpp
    lib/openssl++/certificate.cpp
    lib/openssl++/certificate_store.cpp
    lib/openssl++/certificate_stack.cpp
    lib/openssl++/cms.cpp
    lib/zipsign/file.cpp
    lib/zipsign/partial_input_file.cpp
    lib/zipsign/zip.cpp
    lib/zipsign/signer.cpp
    lib/zipsign/verifier.cpp
    lib/zipsign/informer.cpp
    lib/zipsign/main.cpp
    lib/zipsign/sign.cpp
    lib/zipsign/verify.cpp
    lib/zipsign/info.cpp
    lib/zipsign/base64.cpp
)
target_include_directories(zipsign PUBLIC include)
target_include_directories(zipsign PRIVATE lib ${OPENSSL_INCLUDE_DIRS})
target_compile_options(zipsign PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(zipsign PUBLIC ${OPENSSL_LINK_LIBRARIES})

add_executable(app
    src/zipsign/main.cpp)
set_target_properties(app PROPERTIES OUTPUT_NAME "zipsign")
target_link_libraries(app PRIVATE zipsign)

install(TARGETS app DESTINATION bin)

# Unit tests

if(NOT WITHOUT_TESTS)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include (CTest)

pkg_check_modules(GTEST REQUIRED gtest_main)
include(GoogleTest)
pkg_check_modules(GMOCK REQUIRED gmock)

add_executable(alltests
    test-src/test_sign_and_verify.cpp
    test-src/test_informer.cpp
    test-src/test_file.cpp
    test-src/test_partial_input_file.cpp
    test-src/test_zip.cpp
    test-src/test_base64.cpp
    test-src/openssl/test_basic_io.cpp
    test-src/openssl/test_openssl_exception.cpp
    test-src/openssl/test_openssl_base_exception.cpp
    test-src/openssl/test_file_not_found_exception.cpp
    test-src/openssl/test_certificate.cpp
    test-src/openssl/test_certificate_store.cpp
    test-src/openssl/test_certificate_stack.cpp
    test-src/openssl/test_private_key.cpp
    test-src/openssl/test_cms.cpp
    test-src/testutils.cpp
    test-src/cli/test_print_usage.cpp
    test-src/cli/test_main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/keyring.pem
)

target_include_directories(alltests PRIVATE
    include
    lib
    test
    ${GMOCK_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(alltests PRIVATE zipsign)
target_compile_options(alltests PRIVATE ${GMOCK_CFLAGS} ${GTEST_CFLAGS} "-pthread" "-Wno-sign-compare")
target_link_libraries(alltests PRIVATE zipsign ${GMOCK_LINK_LIBRARIES} ${GTEST_LINK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(OUTPUT
        self-signed/key.pem
        self-signed/cert.pem
        ca/root-ca/private/root-ca.key
        ca/root-ca.crt
        ca/signing-ca/private/signing-ca.key
        ca/signing-ca.crt
        certs/alice.key
        certs/alice.crt
        keyring.pem
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/test-src/openssl/create-pki.sh ${CMAKE_CURRENT_SOURCE_DIR}/test-src/openssl
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/test-src/openssl/create-pki.sh
        ${CMAKE_CURRENT_SOURCE_DIR}/test-src/openssl/root-ca.conf
)


# copy test data
configure_file(test-src/resources/test.zip test.zip COPYONLY)
configure_file(test-src/resources/message.txt message.txt COPYONLY)

enable_testing()
gtest_discover_tests(alltests TEST_PREFIX alltests:)

add_custom_target(check
    ./alltests)
add_dependencies(check alltests)

add_custom_target(memcheck
    valgrind ./alltests)
add_dependencies(memcheck alltests)

add_custom_target(coverage
    mkdir -p coverage
	COMMAND ./alltests
	COMMAND gcovr --html-details coverage/zipsign.html -r .. -e ../test -e ../src 
)
add_dependencies(coverage alltests)

endif(NOT WITHOUT_TESTS)

## CPack

set(CPACK_PACKAGE_VENDOR "Falk Werner")

if(NOT WITHOUT_DEB)

list(APPEND CPACK_GENERATOR "DEB")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Falk Werner <github.com/falk-werner>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS openssl)

endif(NOT WITHOUT_DEB)

include(CPack)
