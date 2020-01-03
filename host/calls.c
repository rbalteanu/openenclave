// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/host.h>
#include <openenclave/internal/raise.h>

#include "calls.h"

/*
**==============================================================================
**
** oe_register_ocall_function_table()
**
** Register an ocall table with the given table_id.
**
**==============================================================================
*/

ocall_table_t _ocall_tables[OE_MAX_OCALL_TABLES];
static oe_mutex _ocall_tables_lock = OE_H_MUTEX_INITIALIZER;

oe_result_t oe_register_ocall_function_table(
    const oe_table_id_t* table_id,
    const oe_ocall_func_t* ocalls,
    size_t num_ocalls)
{
    oe_result_t result = OE_UNEXPECTED;
    size_t index = (size_t)-1;

    oe_mutex_lock(&_ocall_tables_lock);

    /* Find a free index. */
    for (size_t i = 0; i < OE_MAX_OCALL_TABLES; i++)
    {
        const ocall_table_t* table = &_ocall_tables[i];

        if (!table->used)
        {
            index = i;
            break;
        }
    }

    /* If no free entry found. */
    if (index == (size_t)-1)
        OE_RAISE(OE_NOT_FOUND);

    /* Assign the entry. */
    _ocall_tables[index].used = true;
    _ocall_tables[index].table_id = *table_id;
    _ocall_tables[index].ocalls = ocalls;
    _ocall_tables[index].num_ocalls = num_ocalls;

    result = OE_OK;

done:

    oe_mutex_unlock(&_ocall_tables_lock);

    return result;
}

/*
**==============================================================================
**
** oe_call_enclave_function_by_table_id()
**
** Call the enclave function specified by the given table-id and function-id.
**
**==============================================================================
*/

oe_result_t oe_call_enclave_function_by_table_id(
    oe_enclave_t* enclave,
    const oe_table_id_t* table_id,
    uint64_t function_id,
    const void* input_buffer,
    size_t input_buffer_size,
    void* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written)
{
    oe_result_t result = OE_UNEXPECTED;
    oe_call_enclave_function_args_t args;

    /* Reject invalid parameters */
    if (!enclave)
        OE_RAISE(OE_INVALID_PARAMETER);

    /* Initialize the call_enclave_args structure */
    {
        args.table_id = *table_id;
        args.function_id = function_id;
        args.input_buffer = input_buffer;
        args.input_buffer_size = input_buffer_size;
        args.output_buffer = output_buffer;
        args.output_buffer_size = output_buffer_size;
        args.output_bytes_written = 0;
        args.result = OE_UNEXPECTED;
    }

    /* Perform the ECALL */
    {
        uint64_t arg_out = 0;

        OE_CHECK(oe_ecall(
            enclave,
            OE_ECALL_CALL_ENCLAVE_FUNCTION,
            (uint64_t)&args,
            &arg_out));
        OE_CHECK((oe_result_t)arg_out);
    }

    /* Check the result */
    OE_CHECK(args.result);

    *output_bytes_written = args.output_bytes_written;
    result = OE_OK;

done:
    return result;
}

/*
**==============================================================================
**
** oe_call_enclave_function()
**
** Call the enclave function specified by the given function-id in the default
** function table.
**
**==============================================================================
*/

oe_result_t oe_call_enclave_function(
    oe_enclave_t* enclave,
    uint32_t function_id,
    const void* input_buffer,
    size_t input_buffer_size,
    void* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written)
{
    const oe_table_id_t table_id = OE_ZERO_TABLE_ID;

    return oe_call_enclave_function_by_table_id(
        enclave,
        &table_id,
        function_id,
        input_buffer,
        input_buffer_size,
        output_buffer,
        output_buffer_size,
        output_bytes_written);
}
