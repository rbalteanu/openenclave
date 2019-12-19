// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/internal/core/atexit.h>

int atexit(void (*function)(void))
{
    return oe_atexit(function);
}
