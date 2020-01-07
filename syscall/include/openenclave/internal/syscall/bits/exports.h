// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_INTERNAL_EXPORTS_H
#define _OE_SYSCALL_INTERNAL_EXPORTS_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/result.h>
#include <openenclave/bits/types.h>
#include <openenclave/edger8r/enclave.h>

/* The syscall layer depends on these non-exported OE symbols. */

OE_EXTERNC_BEGIN

/*
**==============================================================================
**
** OE function table:
**
**==============================================================================
*/

typedef struct _oe_table_id
{
    uint64_t d1;
    uint64_t d2;
} oe_table_id_t;

oe_result_t oe_call_host_function_by_table_id(
    const oe_table_id_t* table_id,
    size_t function_id,
    const void* input_buffer,
    size_t input_buffer_size,
    void* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written,
    bool switchless);

oe_result_t oe_register_ecall_function_table(
    const oe_table_id_t* table_id,
    const oe_ecall_func_t* ecalls,
    size_t num_ecalls);

/*
**==============================================================================
**
** oelibc syscall hooks:
**
**==============================================================================
*/

typedef oe_result_t (*oe_syscall_hook_t)(
    long number,
    long arg1,
    long arg2,
    long arg3,
    long arg4,
    long arg5,
    long arg6,
    long* ret);

void oe_register_syscall_hook(oe_syscall_hook_t hook);

/*
**==============================================================================
**
** miscellaneous
**
**==============================================================================
*/

int oe_host_printf(const char* fmt, ...);

int oe_sleep_msec(uint64_t milliseconds);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_INTERNAL_EXPORTS_H */
