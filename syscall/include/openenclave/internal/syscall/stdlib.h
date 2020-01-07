// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_STDLIB_H
#define _OE_SYSCALL_STDLIB_H

#include <limits.h>
#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <openenclave/internal/syscall/bits/defs.h>

OE_EXTERNC_BEGIN

typedef struct _oe_syscall_path
{
    char buf[PATH_MAX];
} oe_syscall_path_t;

char* oe_realpath(const char* path, oe_syscall_path_t* resolved_path);

OE_NO_RETURN void oe_exit(int status);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_STDLIB_H */
