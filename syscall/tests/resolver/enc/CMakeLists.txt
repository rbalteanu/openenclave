# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../resolver_test.edl enclave gen --edl-search-dir ../../../edl)

add_enclave(TARGET resolver_enc SOURCES enc.c ${gen})

maybe_build_using_clangw(resolver_enc)

target_include_directories(resolver_enc PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(resolver_enc oelibc oehostresolver oesyscallenclave)
