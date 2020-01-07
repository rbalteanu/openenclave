// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_SYS_IOCTL_H
#define _OE_SYSCALL_SYS_IOCTL_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <stdarg.h>
#include <sys/ioctl.h>

OE_EXTERNC_BEGIN

int __oe_ioctl(int fd, unsigned long request, uint64_t arg);

int oe_ioctl(int fd, unsigned long request, ...);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_SYS_IOCTL_H */
