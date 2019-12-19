// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/enclave.h>
#include <openenclave/internal/core/string.h>
#include "locale_impl.h"

char* strerror_l(int errnum, locale_t loc)
{
    OE_UNUSED(loc);
    oe_assert(loc == C_LOCALE);
    return oe_strerror(errnum);
}

char* strerror(int errnum)
{
    return oe_strerror(errnum);
}
