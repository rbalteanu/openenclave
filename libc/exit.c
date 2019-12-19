// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/bits/defs.h>
#include <openenclave/internal/core/exit.h>
#include <stdlib.h>

OE_NO_RETURN void exit(int code)
{
    _Exit(code);
}
