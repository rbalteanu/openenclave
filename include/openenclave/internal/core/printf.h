// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_INTERNAL_CORE_PRINTF_H
#define _OE_INTERNAL_CORE_PRINTF_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

int oe_vsnprintf(char* str, size_t size, const char* fmt, oe_va_list ap);

int oe_snprintf(char* str, size_t size, const char* format, ...);

OE_EXTERNC_END

#endif /* _OE_INTERNAL_CORE_PRINTF_H */
