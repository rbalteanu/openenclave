// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_REALPATH_H
#define _OE_SYSCALL_REALPATH_H

#include <openenclave/syscall/common.h>

OE_EXTERNC_BEGIN

typedef struct _oe_syscall_path
{
    char buf[OE_PATH_MAX];
} oe_syscall_path_t;

char* oe_realpath(const char* path, oe_syscall_path_t* resolved_path);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_REALPATH_H */
