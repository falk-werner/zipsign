cmake_minimum_required (VERSION 3.1)
project(zipsign VERSION 1.0.0)

option(WITHOUT_TESTS   "disable unit tests"   OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)


set(CMAKE_C_STANDARD 99)
set(C_WARNINGS -Wall -Wextra)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(coverage)

add_library(base64 STATIC
    lib/base64/base64.c
)
target_include_directories(base64 PRIVATE lib include)


add_library(cli STATIC
    lib/cli/app.cc
    lib/cli/argument.cc
    lib/cli/default_arguments.cc
    lib/cli/options.cc
    lib/cli/default_verb.cc
)
target_include_directories(cli PRIVATE lib include)

add_library(openssl++ STATIC
    lib/openssl++/exception.cc
    lib/openssl++/init.cc
    lib/openssl++/basic_io.cc
    lib/openssl++/private_key.cc
    lib/openssl++/certificate.cc
    lib/openssl++/certificate_store.cc
    lib/openssl++/certificate_stack.cc
    lib/openssl++/cms.cc
)
target_include_directories(openssl++ PRIVATE lib include)
target_compile_options(openssl++ PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(openssl++ PUBLIC ${OPENSSL_LIBRARIES})

add_library(zipsign STATIC
        lib/zipsign/file.cc
        lib/zipsign/partial_input_file.cc
        lib/zipsign/zip.cc
        lib/zipsign/signer.cc
        lib/zipsign/verifier.cc
        lib/zipsign/informer.cc
)
target_include_directories(zipsign PRIVATE lib include)
target_compile_options(zipsign PUBLIC ${OPENSSL_CFLAGS_OTHER})
target_link_libraries(zipsign PUBLIC openssl++ base64)

add_executable(app
    src/zipsign/main.cc)
set_target_properties(app PROPERTIES OUTPUT_NAME "zipsign")
target_include_directories(app PRIVATE include src/cli)
target_link_libraries(app PRIVATE zipsign cli)

install(TARGETS app DESTINATION bin)

# Unit tests

if(NOT WITHOUT_TESTS)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include (CTest)

pkg_check_modules(GTEST gtest_main)
include(GoogleTest)
pkg_check_modules(GMOCK gmock)

add_executable(alltests
    test/test_sign_and_verify.cc
    test/test_file.cc
    test/test_partial_input_file.cc
    test/test_zip.cc
    test/base64/test_base64.cc
    test/cli/test_app.cc
    test/cli/test_argument.cc
    test/cli/test_default_arguments.cc
    test/cli/test_options.cc
    test/cli/test_verb.cc
    test/openssl/openssl_environment.cc
    test/openssl/test_basic_io.cc
    test/openssl/test_openssl_exception.cc
    test/openssl/test_openssl_base_exception.cc
    test/openssl/test_file_not_found_exception.cc
    test/openssl/test_certificate.cc
    test/openssl/test_certificate_store.cc
    test/openssl/test_certificate_stack.cc
    test/openssl/test_private_key.cc
    test/openssl/test_cms.cc
)

target_include_directories(alltests PRIVATE
    include
    lib
    test
    ${GMOCK_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(alltests PRIVATE zipsign)
target_compile_options(alltests PRIVATE ${GMOCK_CFLAGS} ${GTEST_CFLAGS} "-pthread")
target_link_libraries(alltests PRIVATE zipsign cli ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(TARGET alltests PRE_BUILD
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/openssl/create-pki.sh ${CMAKE_CURRENT_SOURCE_DIR}/test/openssl
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/test/openssl/create-pki.sh
        ${CMAKE_CURRENT_SOURCE_DIR}/test/openssl/root-ca.conf
)


# copy test data
configure_file(test/resources/test.zip test.zip COPYONLY)
configure_file(test/resources/message.txt message.txt COPYONLY)

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
	COMMAND lcov --initial --capture --directory . --output-file coverage/lcov.info
	COMMAND ./alltests
	COMMAND lcov --capture --directory . --output-file coverage/lcov.info
	COMMAND lcov --remove coverage/lcov.info '/usr/*' --output-file coverage/lcov.info
	COMMAND lcov --remove coverage/lcov.info '*/test/*' --output-file coverage/lcov.info
)
add_dependencies(coverage alltests)

add_custom_target(coverage-report
	COMMAND genhtml -branch-coverage --highlight --legend coverage/lcov.info --output-directory coverage/report
)
add_dependencies(coverage-report coverage)

endif(NOT WITHOUT_TESTS)
