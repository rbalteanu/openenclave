# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

add_library(oehostsock STATIC hostsock.c)

maybe_build_using_clangw(oehostsock)

add_dependencies(oehostsock syscall_trusted_edl)

target_include_directories(oehostsock PRIVATE
    ${CMAKE_BINARY_DIR}/syscall/enclave
    ${PROJECT_SOURCE_DIR}/syscall/include
    ${PROJECT_SOURCE_DIR}/syscall/include/edger8r)

target_link_libraries(oehostsock oesyscallenclave)

install(TARGETS oehostsock EXPORT openenclave-targets ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/enclave)
