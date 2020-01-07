// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_BITS_DEFS_H
#define _OE_SYSCALL_BITS_DEFS_H

#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

#ifndef OE_NO_RETURN
#ifdef __GNUC__
#define OE_NO_RETURN __attribute__((__noreturn__))
#else
#define OE_NO_RETURN
#endif
#endif

OE_EXTERNC_END

#endif // _OE_SYSCALL_BITS_DEFS_H
