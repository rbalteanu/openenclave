// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_FCNTL_H
#define _OE_SYSCALL_FCNTL_H

#include <fcntl.h>
#include <openenclave/bits/defs.h>
#include <openenclave/internal/syscall/bits/types.h>
#include <stdarg.h>

OE_EXTERNC_BEGIN

#define __OE_FLOCK oe_flock
#include <openenclave/internal/syscall/bits/flock.h>
#undef __OE_FLOCK

#define __OE_FLOCK oe_flock64
#include <openenclave/internal/syscall/bits/flock.h>
#undef __OE_FLOCK

struct oe_f_owner_ex
{
    int type;
    oe_pid_t pid;
};

int oe_open(const char* pathname, int flags, oe_mode_t mode);

int oe_open_d(uint64_t devid, const char* pathname, int flags, oe_mode_t mode);

int __oe_fcntl(int fd, int cmd, uint64_t arg);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_FCNTL_H */
