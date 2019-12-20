// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_STRINGS_H
#define _OE_STRINGS_H

#include <openenclave/internal/core/string.h>

OE_INLINE size_t strlen(const char* s)
{
    return oe_strlen(s);
}

#endif /* _OE_STRINGS_H */
