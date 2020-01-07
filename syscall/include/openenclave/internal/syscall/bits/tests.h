// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_INTERNAL_TESTS_H
#define _OE_SYSCALL_INTERNAL_TESTS_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/result.h>

#ifdef OE_BUILD_ENCLAVE
#include <openenclave/internal/syscall/bits/exports.h>
#define __OE_PRINTF oe_host_printf
#define __OE_ABORT oe_abort
#else
#include <stdio.h>
#include <stdlib.h>
#define __OE_PRINTF printf
#define __OE_ABORT abort
#endif

#define OE_TEST(COND)                           \
    do                                          \
    {                                           \
        if (!(COND))                            \
        {                                       \
            __OE_PRINTF(                        \
                "Test failed: %s(%u): %s %s\n", \
                __FILE__,                       \
                __LINE__,                       \
                __FUNCTION__,                   \
                #COND);                         \
            __OE_ABORT();                       \
        }                                       \
    } while (0)

OE_EXTERNC_BEGIN

uint32_t oe_get_create_flags(void);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_INTERNAL_TESTS_H */
