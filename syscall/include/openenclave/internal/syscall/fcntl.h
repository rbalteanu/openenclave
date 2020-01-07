// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_FCNTL_H
#define _OE_SYSCALL_FCNTL_H

#include <fcntl.h>
#include <openenclave/bits/defs.h>
#include <openenclave/internal/syscall/bits/types.h>
#include <stdarg.h>

OE_EXTERNC_BEGIN

struct oe_flock
{
    short l_type;
    short l_whence;
    oe_off_t l_start;
    oe_off_t l_len;
    oe_pid_t l_pid;
};

#define oe_flock64 oe_flock

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
