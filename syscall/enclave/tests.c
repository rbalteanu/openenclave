// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#define _GNU_SOURCE

#include <openenclave/internal/syscall/bits/exports.h>
#include <openenclave/internal/syscall/bits/tests.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int oe_host_vfprintf(int device, const char* fmt, va_list ap);

void __oe_test_fail(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    oe_host_vfprintf(1, format, ap);
    va_end(ap);
    abort();
}
