// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_SYS_SYSCALL_H
#define _OE_SYSCALL_SYS_SYSCALL_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <sys/syscall.h>

OE_EXTERNC_BEGIN

long oe_syscall(long number, ...);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_SYS_SYSCALL_H */
