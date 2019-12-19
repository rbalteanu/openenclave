# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

add_library(oehostresolver STATIC hostresolver.c)

maybe_build_using_clangw(oehostresolver)

add_dependencies(oehostresolver syscall_trusted_edl)

target_include_directories(oehostresolver PRIVATE
    ${CMAKE_BINARY_DIR}/syscall/enclave
    ${PROJECT_SOURCE_DIR}/syscall/include
    ${PROJECT_SOURCE_DIR}/syscall/include/edger8r)

target_link_libraries(oehostresolver oesyscallenclave)

install(TARGETS oehostresolver EXPORT openenclave-targets ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/enclave)
