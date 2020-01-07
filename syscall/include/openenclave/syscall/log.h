// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_LOG_H
#define _OE_SYSCALL_LOG_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

#ifndef oe_syscall_va_list
#define oe_syscall_va_list __builtin_va_list
#endif

typedef enum _oe_syscall_log_level
{
    OE_SYSCALL_LOG_LEVEL_NONE = 0,
    OE_SYSCALL_LOG_LEVEL_FATAL,
    OE_SYSCALL_LOG_LEVEL_ERROR,
    OE_SYSCALL_LOG_LEVEL_WARNING,
    OE_SYSCALL_LOG_LEVEL_INFO,
    OE_SYSCALL_LOG_LEVEL_VERBOSE,
    OE_SYSCALL_LOG_LEVEL_MAX
} oe_syscall_log_level_t;

typedef void (*oe_syscall_log_hook_t)(
    oe_syscall_log_level_t level,
    oe_syscall_va_list ap);

void oe_syscall_log(oe_syscall_log_level_t level, const char* fmt, ...);

void oe_syscall_install_log_hook(oe_syscall_log_hook_t hook);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_LOG_H */
