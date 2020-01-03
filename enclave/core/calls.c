// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/enclave.h>
#include <openenclave/internal/calls.h>
#include <openenclave/internal/raise.h>
#include <openenclave/internal/thread.h>

#include "calls.h"

/* If true, disable the debug malloc checking */
bool oe_disable_debug_malloc_check;

/*
**==============================================================================
**
** oe_register_ecall_function_table()
**
** Register an ecall table with the given table_id.
**
**==============================================================================
*/

ecall_table_t _ecall_tables[OE_MAX_ECALL_TABLES];
static oe_spinlock_t _ecall_tables_lock = OE_SPINLOCK_INITIALIZER;

oe_result_t oe_register_ecall_function_table(
    const oe_table_id_t* table_id,
    const oe_ecall_func_t* ecalls,
    size_t num_ecalls)
{
    oe_result_t result = OE_UNEXPECTED;
    size_t index = (size_t)-1;

    oe_spin_lock(&_ecall_tables_lock);

    /* Find a free index. */
    for (size_t i = 0; i < OE_MAX_ECALL_TABLES; i++)
    {
        const ecall_table_t* table = &_ecall_tables[i];

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
    _ecall_tables[index].used = true;
    _ecall_tables[index].table_id = *table_id;
    _ecall_tables[index].ecalls = ecalls;
    _ecall_tables[index].num_ecalls = num_ecalls;

    result = OE_OK;

done:

    oe_spin_unlock(&_ecall_tables_lock);

    return result;
}
