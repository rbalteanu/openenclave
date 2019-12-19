# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

include(oeedl_file)

oeedl_file(../socketpair_test.edl host gen --edl-search-dir "../../../edl")

add_executable(socketpair_host host.c ${gen})

target_include_directories(socketpair_host PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(socketpair_host oesyscallhost oehostapp)
