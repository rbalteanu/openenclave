# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../socketpair_test.edl enclave gen --edl-search-dir "../../../edl")

add_enclave(TARGET socketpair_enc SOURCES enc.c ${gen})

target_include_directories(socketpair_enc PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(socketpair_enc oelibc oesyscallenclave oehostsock)
