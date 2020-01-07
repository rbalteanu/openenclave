// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_INTERNAL_TESTS_H
#define _OE_SYSCALL_INTERNAL_TESTS_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

OE_PRINTF_FORMAT(1, 2)
void __oe_test_fail(const char* format, ...);

#define OE_TEST(COND)                           \
    do                                          \
    {                                           \
        if (!(COND))                            \
        {                                       \
            __oe_test_fail(                     \
                "Test failed: %s(%u): %s %s\n", \
                __FILE__,                       \
                __LINE__,                       \
                __FUNCTION__,                   \
                #COND);                         \
        }                                       \
    } while (0)

uint32_t oe_get_create_flags(void);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_INTERNAL_TESTS_H */
