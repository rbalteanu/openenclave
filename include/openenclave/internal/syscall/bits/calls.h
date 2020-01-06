// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_BITS_CALLS_H
#define _OE_SYSCALL_BITS_CALLS_H

#include <openenclave/internal/calls.h>

OE_EXTERNC_BEGIN

#define OE_SYSCALL_FUNCTION_TABLE_ID           \
    {                                          \
        0x034ce7c58d0b4e47, 0xb6118fbc095f245e \
    }
#define OE_SYSCALL_OCALL_FUNCTION_TABLE_ID OE_SYSCALL_FUNCTION_TABLE_ID
#define OE_SYSCALL_ECALL_FUNCTION_TABLE_ID OE_SYSCALL_FUNCTION_TABLE_ID

OE_EXTERNC_END

#endif // _OE_SYSCALL_BITS_CALLS_H
