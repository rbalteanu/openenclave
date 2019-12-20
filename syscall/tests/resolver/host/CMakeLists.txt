# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

include(oeedl_file)

oeedl_file(../resolver_test.edl host gen --edl-search-dir "../../../edl")

add_executable(resolver_host host.c ${gen})

target_include_directories(resolver_host PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(resolver_host oesyscallhost oehostapp)
