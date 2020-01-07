// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_RAISE_H
#define _OE_SYSCALL_RAISE_H

#include <errno.h>
#include <openenclave/bits/result.h>
#include <openenclave/internal/syscall/bits/exports.h>
#include <openenclave/internal/syscall/device.h>
#include <openenclave/syscall/log.h>

OE_EXTERNC_BEGIN

// clang-format off
#define OE_RAISE_ERRNO(ERRNO)                         \
    do                                                \
    {                                                 \
        int __err = ERRNO;                            \
        oe_syscall_log(OE_SYSCALL_LOG_LEVEL_ERROR,    \
            "errno=%d [%s %s:%d]\n",                  \
            __err, __FILE__, __FUNCTION__, __LINE__); \
        errno = __err;                                \
        goto done;                                    \
    }                                                 \
    while (0)
// clang-format on

// clang-format off
#define OE_RAISE_ERRNO_MSG(ERRNO, FMT, ...)                          \
    do                                                               \
    {                                                                \
        int __err = ERRNO;                                           \
        oe_syscall_log(OE_SYSCALL_LOG_LEVEL_ERROR,                   \
            FMT " errno=%d [%s %s:%d]\n",                            \
            ##__VA_ARGS__, __err, __FILE__, __FUNCTION__, __LINE__); \
        errno = __err;                                               \
        goto done;                                                   \
    }                                                                \
    while (0)
// clang-format on

OE_EXTERNC_END

#endif // _OE_SYSCALL_RAISE_H
