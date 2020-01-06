# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

##==============================================================================
##
## These rules generate the edge routines for the SYSCALL interface, which is
## installed by oe_register_syscall_ecall_function_table().
##
##==============================================================================

set(EDL_DIR ${PROJECT_SOURCE_DIR}/syscall/edl)
set(EDL_FILE ${EDL_DIR}/syscall.edl)

add_custom_command(
    OUTPUT syscall_u.h syscall_u.c syscall_args.h
    DEPENDS ${EDL_FILE} edger8r
    COMMAND edger8r --search-path ${EDL_DIR} --untrusted ${EDL_FILE})

add_custom_target(syscall_untrusted_edl
    DEPENDS syscall_u.h syscall_u.c syscall_args.h)

##==============================================================================
##
## These rules build the oesyscallhost target.
##
##==============================================================================

set(TARGET oesyscallhost)

add_library(${TARGET} STATIC
    syscall_u_wrapper.c
    linux/syscall.c)

maybe_build_using_clangw(${TARGET})

add_dependencies(${TARGET} syscall_untrusted_edl)

target_include_directories(${TARGET} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/syscall/include)

install(TARGETS ${TARGET} EXPORT openenclave-targets ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/host)
