// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_CORELIBC_UNISTD_H
#define _OE_CORELIBC_UNISTD_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <openenclave/corelibc/bits/types.h>

OE_EXTERNC_BEGIN

void* oe_sbrk(intptr_t increment);

OE_EXTERNC_END

#endif /* _OE_CORELIBC_UNISTD_H */
