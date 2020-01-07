// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_CORELIBC_BITS_TYPES_H
#define _OE_CORELIBC_BITS_TYPES_H

#include <openenclave/bits/types.h>

typedef int64_t oe_off_t;

#if defined(OE_NEED_STDC_NAMES)

typedef oe_off_t off_t;

#endif /* defined(OE_NEED_STDC_NAMES) */

#endif /* _OE_CORELIBC_BITS_TYPES_H */
