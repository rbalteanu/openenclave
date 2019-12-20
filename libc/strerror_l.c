// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <assert.h>
#include <string.h>
#include "locale_impl.h"

char* strerror_l(int errnum, locale_t loc)
{
    (void)loc;
    assert(loc == C_LOCALE);
    return strerror(errnum);
}
