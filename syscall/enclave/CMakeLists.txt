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
    OUTPUT syscall_t.h syscall_t.c syscall_args.h
    DEPENDS ${EDL_FILE} edger8r
    COMMAND edger8r --search-path ${EDL_DIR} --trusted ${EDL_FILE})

add_custom_target(syscall_trusted_edl
    DEPENDS syscall_t.h syscall_t.c syscall_args.h)

##==============================================================================
##
## These rules build the ${TARGET} target.
##
##==============================================================================

set(TARGET oesyscallenclave)

add_library(${TARGET} STATIC
    consolefs.c
    device.c
    dirent.c
    ioctl.c
    fcntl.c
    fdtable.c
    iov.c
    mount.c
    musl.c
    netdb.c
    poll.c
    epoll.c
    select.c
    socket.c
    stat.c
    stdio.c
    stdlib.c
    stub.c
    strdup.c
    strtok_r.c
    syscall.c
    unistd.c
    utsname.c)

maybe_build_using_clangw(${TARGET})

add_dependencies(${TARGET} syscall_trusted_edl)

target_include_directories(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include
    ${PROJECT_SOURCE_DIR}/syscall/include/edger8r
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${TARGET} oelibc oecore)

install(TARGETS ${TARGET} EXPORT openenclave-targets ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/openenclave/enclave)
