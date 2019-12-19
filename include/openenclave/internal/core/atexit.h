// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_INTERNAL_CORE_ATEXIT_H
#define _OE_INTERNAL_CORE_ATEXIT_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

int oe_atexit(void (*function)(void));

OE_EXTERNC_END

#endif /* _OE_INTERNAL_CORE_ATEXIT_H */
