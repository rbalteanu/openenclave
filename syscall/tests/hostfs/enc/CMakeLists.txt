# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../test_hostfs.edl enclave gen --edl-search-dir "../../../edl")

add_enclave(TARGET hostfs_enc SOURCES enc.c main.c ${gen})

target_include_directories(hostfs_enc PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(hostfs_enc oelibc oesyscallenclave oehostfs oeenclave)
