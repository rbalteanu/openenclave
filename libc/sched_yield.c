// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/internal/core/sched.h>

int sched_yield(void)
{
    return oe_sched_yield();
}
