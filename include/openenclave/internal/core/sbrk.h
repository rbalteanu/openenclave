// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_INTERNAL_CORE_SBRK_H
#define _OE_INTERNAL_CORE_SBRK_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

void* oe_sbrk(ptrdiff_t increment);

OE_EXTERNC_END

#endif /* _OE_INTERNAL_CORE_SBRK_H */
