# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

# Create a binary for each testcase listed in ../tests.supported
# These tests generate a large number of warnings, some of them specific to compiler versions.
# Disabling warning-as-errors for the specific warnings is painful across multiple different compilers.
# It was agreed to, that it is best to just disable warnings-as-errors for these tests specifically.

oeedl_file(../libunwind.edl enclave gen)

set(LIBUNWIND_TEST_DIR ${PROJECT_SOURCE_DIR}/3rdparty/libunwind/libunwind/tests)

# helper lib to contain file needed by some tests
add_library(libunwindtest-support
        ${LIBUNWIND_TEST_DIR}/ident.c
        ${LIBUNWIND_TEST_DIR}/flush-cache.S
        ${LIBUNWIND_TEST_DIR}/test-static-link-gen.c
        ${LIBUNWIND_TEST_DIR}/flush-cache.h
        test_support.c
        )
target_link_libraries(libunwindtest-support PRIVATE oeenclave oelibc)
target_compile_options(libunwindtest-support PRIVATE -Wno-error -DUNW_LOCAL_ONLY=1)
target_include_directories(libunwindtest-support PUBLIC
        ${PROJECT_SOURCE_DIR}/3rdparty/libunwind
        ${PROJECT_BINARY_DIR}/3rdparty/libunwind
        ${PROJECT_SOURCE_DIR}/3rdparty/libunwind/libunwind/include
    )

function(add_libunwind_test_enc NAME TESTFILE)
    add_enclave(TARGET libunwindtest-${NAME}_enc UUID 061c1ea0-22b3-4194-a8cd-44242462f22a CXX
        SOURCES
        enc.c
        ${gen}
        ${PROJECT_SOURCE_DIR}/${TESTFILE}
        )
    target_include_directories(libunwindtest-${NAME}_enc PRIVATE
        ..
        ${PROJECT_BINARY_DIR}/3rdparty/libunwind/libunwind/tests
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
    target_compile_options(libunwindtest-${NAME}_enc
        PRIVATE
        -Wno-error
        -Wno-macro-redefined # We need `UNW_LOCAL_ONLY=1` but some files already define it.
        -fno-inline-functions)
    if(("${NAME}" STREQUAL "Ltest-varargs") AND ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
        target_compile_options(libunwindtest-${NAME}_enc PRIVATE -include enc/clang_varargs_extra.h)
    endif()
    target_compile_definitions(libunwindtest-${NAME}_enc PRIVATE
        -DWITH_MAIN -D__TEST__="${TESTFILE}" -DUNW_LOCAL_ONLY=1)
    target_link_libraries(libunwindtest-${NAME}_enc libunwindtest-support)
    target_link_libraries(libunwindtest-${NAME}_enc -Wl,--undefined=pthread_mutex_lock)
    target_link_libraries(libunwindtest-${NAME}_enc -Wl,--undefined=_U_dyn_info_list_addr)
endfunction(add_libunwind_test_enc)

file(STRINGS "../tests.supported" alltests)
foreach(testcase ${alltests})
    get_testcase_name(${testcase} name "3rdparty/libunwind/libunwind/tests/")
    add_libunwind_test_enc("${name}" "${testcase}")
endforeach(testcase)
