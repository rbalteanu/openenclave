# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.


oeedl_file(../test_dup.edl host gen --edl-search-dir "../../../edl")

add_executable(dup_host host.c ${gen})

target_include_directories(dup_host PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(dup_host oesyscallhost oehostapp)
