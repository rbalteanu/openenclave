// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_STDLIB_H
#define _OE_SYSCALL_STDLIB_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <openenclave/internal/syscall/bits/defs.h>

OE_EXTERNC_BEGIN

#ifndef OE_PATH_MAX
#define OE_PATH_MAX 4096
#endif

typedef struct _oe_syscall_path
{
    char buf[OE_PATH_MAX];
} oe_syscall_path_t;

char* oe_realpath(const char* path, oe_syscall_path_t* resolved_path);

OE_NO_RETURN void oe_exit(int status);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_STDLIB_H */
