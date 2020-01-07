// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_STDIO_H
#define _OE_SYSCALL_STDIO_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

int oe_rename(const char* oldpath, const char* newpath);

int oe_rename_d(uint64_t devid, const char* oldpath, const char* newpath);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_STDIO_H */
