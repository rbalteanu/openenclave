// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_BITS_DEFS_H
#define _OE_SYSCALL_BITS_DEFS_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

#ifndef OE_NO_RETURN
#ifdef __GNUC__
#define OE_NO_RETURN __attribute__((__noreturn__))
#else
#define OE_NO_RETURN
#endif
#endif

#ifdef __GNUC__
#define OE_UNUSED_ATTRIBUTE __attribute__((unused))
#elif _MSC_VER
#define OE_UNUSED_ATTRIBUTE
#else
#error OE_UNUSED_ATTRIBUTE not implemented
#endif

#define __OE_CONCAT(X, Y) X##Y
#define OE_CONCAT(X, Y) __OE_CONCAT(X, Y)

#define OE_STATIC_ASSERT(COND)       \
    typedef unsigned char OE_CONCAT( \
        __OE_STATIC_ASSERT, __LINE__)[(COND) ? 1 : -1] OE_UNUSED_ATTRIBUTE

OE_EXTERNC_END

#endif // _OE_SYSCALL_BITS_DEFS_H
