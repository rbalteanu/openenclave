# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

##==============================================================================
##
## These rules generate the edge routines for the SYSCALL interface, which is
## installed by oe_register_syscall_ecall_function_table().
##
##==============================================================================

set(SYSCALL_EDL ${EDL_DIR}/syscall.edl)

add_custom_command(
    OUTPUT syscall_u.h syscall_u.c syscall_args.h
    DEPENDS ${SYSCALL_EDL} edger8r
    COMMAND edger8r --search-path ${EDL_DIR} --untrusted ${SYSCALL_EDL})

add_custom_target(syscall_untrusted_edl
    DEPENDS syscall_u.h syscall_u.c syscall_args.h)

add_dependencies(oehost syscall_untrusted_edl)
