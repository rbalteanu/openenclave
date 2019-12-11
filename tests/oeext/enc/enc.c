// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/bits/ext.h>
#include <openenclave/enclave.h>
#include <openenclave/internal/rsa.h>
#include <openenclave/internal/tests.h>
#include <stdio.h>
#include <string.h>
#include "oeext_t.h"

/* The 'oeext registration' subcommand fills this in. */
OE_EXT_SECTION oe_ext_registration_t registration;

void dump_registration_ecall(void)
{
    oe_ext_dump_registration(&registration);
}

void verify_ecall(
    const struct _oe_ext_sigstruct* sigstruct,
    const struct _oe_ext_hash* extmeasure)
{
    oe_result_t r;

    oe_ext_dump_hash("extmeasure", extmeasure);

    /* Dump the structure. */
    oe_ext_dump_sigstruct(sigstruct);

    r = oe_ext_verify_signature(
        &registration.pubkey,
        &registration.extid,
        extmeasure,
        &sigstruct->signature);
    OE_TEST(r == OE_OK);

    printf("=== VERIFY OKAY\n");
}

OE_SET_ENCLAVE_SGX(
    1,    /* ProductID */
    1,    /* SecurityVersion */
    true, /* AllowDebug */
    1024, /* HeapPageCount */
    1024, /* StackPageCount */
    2);   /* TCSCount */
