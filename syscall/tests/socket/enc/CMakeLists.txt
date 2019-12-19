# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../socket_test.edl enclave gen --edl-search-dir "../../../edl")

add_enclave(TARGET socket_enc SOURCES enc.c ${gen})

target_include_directories(socket_enc PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(socket_enc oesyscallenclave oelibc oehostsock)
