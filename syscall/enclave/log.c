// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/internal/syscall/bits/exports.h>
#include <openenclave/internal/syscall/stdarg.h>
#include <openenclave/syscall/log.h>

static oe_syscall_log_hook_t _hook;

void oe_syscall_log(oe_syscall_log_level_t level, const char* fmt, ...)
{
    if (_hook)
    {
        oe_va_list ap;

        oe_va_start(ap, fmt);
        (*_hook)(level, ap);
        oe_va_end(ap);
    }
}

void oe_syscall_install_log_hook(oe_syscall_log_hook_t hook)
{
    _hook = hook;
}
