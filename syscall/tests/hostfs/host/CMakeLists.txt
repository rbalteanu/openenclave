# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../test_hostfs.edl host gen --edl-search-dir "../../../edl")

add_executable(hostfs_host host.c ${gen})

target_include_directories(hostfs_host PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(hostfs_host oesyscallhost oehostapp)
