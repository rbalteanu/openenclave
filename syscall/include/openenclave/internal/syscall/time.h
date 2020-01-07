// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_TIME_H
#define _OE_SYSCALL_TIME_H

#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

struct oe_timespec
{
    time_t tv_sec;
    long tv_nsec;
};

OE_EXTERNC_END

#endif /* _OE_SYSCALL_TIME_H */
