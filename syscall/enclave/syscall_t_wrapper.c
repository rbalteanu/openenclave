// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/enclave.h>

#include <openenclave/edger8r/enclave.h>
#include <openenclave/internal/syscall/bits/calls.h>
#include <openenclave/internal/syscall/bits/exports.h>
#include <openenclave/internal/syscall/stdio.h>
#include <openenclave/internal/syscall/string.h>
#include <pthread.h>

/* Rename the ecalls table. */
#define __oe_ecalls_table __oe_syscall_ecalls_table
#define __oe_ecalls_table_size __oe_syscall_ecalls_table_size

/* Override oe_call_host_function() calls with _call_host_function(). */
#define oe_call_host_function _call_host_function

/* Use this function below instead of oe_call_host_function(). */
static oe_result_t _call_host_function(
    size_t function_id,
    const void* input_buffer,
    size_t input_buffer_size,
    void* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written)
{
    const oe_table_id_t table_id = OE_SYSCALL_OCALL_FUNCTION_TABLE_ID;

    return oe_call_host_function_by_table_id(
        &table_id,
        function_id,
        input_buffer,
        input_buffer_size,
        output_buffer,
        output_buffer_size,
        output_bytes_written,
        false /* non-switchless */);
}

#include "syscall_t.c"

static pthread_once_t _once = PTHREAD_ONCE_INIT;

static void _once_function(void)
{
    const oe_table_id_t table_id = OE_SYSCALL_ECALL_FUNCTION_TABLE_ID;

    if (oe_register_ecall_function_table(
            &table_id,
            __oe_syscall_ecalls_table,
            __oe_syscall_ecalls_table_size) != OE_OK)
    {
        oe_host_printf(
            "%s(%u): failed to register syscall function table\n",
            __FILE__,
            __LINE__);
        oe_abort();
    }
}

void oe_register_syscall_ecall_function_table(void)
{
    pthread_once(&_once, _once_function);
}
