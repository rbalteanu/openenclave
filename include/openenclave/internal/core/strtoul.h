// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_INTERNAL_CORE_STRTOUL_H
#define _OE_INTERNAL_CORE_STRTOUL_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

unsigned long int oe_strtoul(const char* nptr, char** endptr, int base);

OE_EXTERNC_END

#endif /* _OE_INTERNAL_CORE_STRTOUL_H */
