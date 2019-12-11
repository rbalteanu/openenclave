// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <ctype.h>
#include <openenclave/bits/ext.h>
#include <openenclave/internal/files.h>
#include <openenclave/internal/raise.h>
#include <openenclave/internal/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../host/crypto/rsa.h"

oe_result_t _ascii_to_binary(const char* ascii, uint8_t* data, size_t size)
{
    oe_result_t result = OE_UNEXPECTED;
    const char* p = ascii;

    if (!ascii || !data || !size)
        OE_RAISE(OE_INVALID_PARAMETER);

    memset(data, 0, size);

    if (strlen(ascii) != 2 * size)
        OE_RAISE(OE_FAILURE);

    for (size_t i = 0; i < size; i++)
    {
        unsigned int byte;
        int n;

        n = sscanf(p, "%02x", &byte);

        if (n != 1)
            OE_RAISE(OE_FAILURE);

        data[i] = (uint8_t)byte;
        p += 2;
    }

    result = OE_OK;

done:
    return result;
}

oe_result_t oe_ext_ascii_to_hash(const char* ascii, oe_ext_hash_t* hash)
{
    return _ascii_to_binary(ascii, hash->buf, sizeof(oe_ext_hash_t));
}

oe_result_t oe_ext_load_sigstruct(
    const char* path,
    oe_ext_sigstruct_t* sigstruct)
{
    oe_result_t result = OE_UNEXPECTED;
    FILE* is = NULL;
    char line[4096];

    if (sigstruct)
        memset(sigstruct, 0, sizeof(oe_ext_sigstruct_t));

    if (!path || !sigstruct)
        OE_RAISE(OE_INVALID_PARAMETER);

    if (!(is = fopen(path, "r")))
        OE_RAISE(OE_NOT_FOUND);

    while (fgets(line, sizeof(line), is) != NULL)
    {
        size_t len = strlen(line);
        char* p = line;
        char* end = p + len;
        const char* name;
        char* name_end;

        if (len == sizeof(line) - 1)
            OE_RAISE(OE_FAILURE);

        /* Remove leading whitespace */
        while (isspace(*p))
            p++;

        /* Remove trailing whitespace */
        while (end != p && isspace(end[-1]))
            *--end = '\0';

        /* Skip comment lines and empty */
        if (p[0] == '#' || p[0] == '\0')
            continue;

        /* Read the name */
        {
            const char* start = p;

            if (!(isalpha(*p) || *p == '_'))
                OE_RAISE(OE_FAILURE);

            p++;

            while (isalnum(*p) || *p == '_')
                p++;

            name = start;
            name_end = p;
        }

        /* Expect equal */
        {
            /* Skip whitespace. */
            while (isspace(*p))
                p++;

            /* Expect '=' characer */
            if (*p++ != '=')
                OE_RAISE(OE_FAILURE);

            /* Skip whitespace. */
            while (isspace(*p))
                p++;
        }

        /* Null-terminate the name now */
        *name_end = '\0';

        /* Handle the value */
        if (strcmp(name, "signer") == 0)
        {
            OE_CHECK(_ascii_to_binary(
                p, sigstruct->signer.buf, sizeof(sigstruct->signer)));
        }
        else if (strcmp(name, "extid") == 0)
        {
            OE_CHECK(_ascii_to_binary(
                p, sigstruct->extid.buf, sizeof(sigstruct->extid)));
        }
        else if (strcmp(name, "extmeasure") == 0)
        {
            OE_CHECK(_ascii_to_binary(
                p, sigstruct->extmeasure.buf, sizeof(sigstruct->extmeasure)));
        }
        else if (strcmp(name, "signature") == 0)
        {
            OE_CHECK(_ascii_to_binary(
                p, sigstruct->signature.buf, sizeof(sigstruct->signature)));
        }
        else
        {
            OE_RAISE(OE_FAILURE);
        }
    }

    result = OE_OK;

done:

    if (is)
        fclose(is);

    return result;
}

static oe_result_t _put(
    FILE* os,
    const char* name,
    const uint8_t* data,
    size_t size)
{
    oe_result_t result = OE_UNEXPECTED;

    if ((size_t)fprintf(os, "%s=", name) != strlen(name) + 1)
        OE_RAISE(OE_FAILURE);

    for (size_t i = 0; i < size; i++)
    {
        if (fprintf(os, "%02x", data[i]) != 2)
            OE_RAISE(OE_FAILURE);
    }

    if (fprintf(os, "\n") != 1)
        OE_RAISE(OE_FAILURE);

    result = OE_OK;

done:
    return result;
}

oe_result_t oe_ext_save_sigstruct(
    const char* path,
    const oe_ext_sigstruct_t* sigstruct)
{
    oe_result_t result = OE_UNEXPECTED;
    FILE* os = NULL;
    const oe_ext_sigstruct_t* p = sigstruct;

    if (!path || !sigstruct)
        OE_RAISE(OE_INVALID_PARAMETER);

    if (!(os = fopen(path, "w")))
        OE_RAISE(OE_NOT_FOUND);

    fprintf(os, "# sigstruct\n");

    OE_CHECK(_put(os, "signer", p->signer.buf, sizeof(p->signer)));
    OE_CHECK(_put(os, "extid", p->extid.buf, sizeof(p->extid)));
    OE_CHECK(_put(os, "extmeasure", p->extmeasure.buf, sizeof(p->extmeasure)));
    OE_CHECK(_put(os, "signature", p->signature.buf, sizeof(p->signature)));

    result = OE_OK;

done:

    if (os)
        fclose(os);

    return result;
}

static void _mem_reverse(void* dest_, const void* src_, size_t n)
{
    unsigned char* dest = (unsigned char*)dest_;
    const unsigned char* src = (const unsigned char*)src_;
    const unsigned char* end = src + n;

    while (n--)
        *dest++ = *--end;
}

static oe_result_t _get_modulus(
    const oe_rsa_public_key_t* rsa,
    uint8_t modulus[OE_EXT_KEY_SIZE])
{
    oe_result_t result = OE_UNEXPECTED;
    uint8_t buf[OE_EXT_KEY_SIZE];
    size_t bufsize = sizeof(buf);

    if (!rsa || !modulus)
        OE_RAISE(OE_INVALID_PARAMETER);

    OE_CHECK(oe_rsa_public_key_get_modulus(rsa, buf, &bufsize));

    /* RSA key length is the modulus length, so these have to be equal. */
    if (bufsize != OE_EXT_KEY_SIZE)
        OE_RAISE(OE_FAILURE);

    _mem_reverse(modulus, buf, bufsize);

    result = OE_OK;

done:
    return result;
}

static oe_result_t _get_exponent(
    const oe_rsa_public_key_t* rsa,
    uint8_t exponent[OE_EXT_EXPONENT_SIZE])
{
    oe_result_t result = OE_UNEXPECTED;
    uint8_t buf[OE_EXT_EXPONENT_SIZE];
    size_t bufsize = sizeof(buf);

    if (!rsa || !exponent)
        OE_RAISE(OE_INVALID_PARAMETER);

    OE_CHECK(oe_rsa_public_key_get_exponent(rsa, buf, &bufsize));

    /* Exponent is in big endian. So, we need to reverse. */
    _mem_reverse(exponent, buf, bufsize);

    /* We zero out the rest to get the right exponent in little endian. */
    memset(exponent + bufsize, 0, OE_EXT_EXPONENT_SIZE - bufsize);

    result = OE_OK;

done:
    return result;
}

static void _compute_hash(
    oe_ext_hash_t* hash,
    const void* p1,
    size_t n1,
    const void* p2,
    size_t n2)
{
    oe_sha256_context_t context;
    OE_SHA256 sha256;

    oe_sha256_init(&context);
    oe_sha256_update(&context, p1, n1);

    if (p2 && n2)
        oe_sha256_update(&context, p2, n2);

    oe_sha256_final(&context, &sha256);
    memcpy(hash, sha256.buf, OE_SHA256_SIZE);
}

oe_result_t oe_ext_sign(
    const char* privkey_path,
    const oe_ext_hash_t* extid,
    const oe_ext_hash_t* extmeasure,
    oe_ext_sigstruct_t* sigstruct)
{
    oe_result_t result = OE_UNEXPECTED;
    oe_rsa_private_key_t privkey;
    bool privkey_initialized = false;
    oe_rsa_public_key_t pubkey;
    bool pubkey_initialized = false;
    void* data = NULL;
    size_t size;
    uint8_t signature[OE_EXT_KEY_SIZE];

    if (sigstruct)
        memset(sigstruct, 0, sizeof(oe_ext_sigstruct_t));

    /* Reject null parameters. */
    if (!privkey_path || !extid || !extmeasure || !sigstruct)
        OE_RAISE(OE_INVALID_PARAMETER);

    /* Load the private RSA key. */
    {
        if (__oe_load_file(privkey_path, 1, &data, &size))
            OE_RAISE(OE_FAILURE);

        size++;

        if (oe_rsa_private_key_read_pem(&privkey, data, size) != OE_OK)
            OE_RAISE(OE_FAILURE);
    }

    /* Load the public key. */
    {
        if (oe_rsa_get_public_key_from_private(&privkey, &pubkey) != OE_OK)
            OE_RAISE(OE_FAILURE);
        pubkey_initialized = true;
    }

    /* Perform the signing operation. */
    {
        oe_ext_hash_t hash;

        /* Compute the composit hash of EXTID and EXTMEASURE. */
        _compute_hash(
            &hash, extid, sizeof(*extid), extmeasure, sizeof(*extmeasure));

        /* Create the signature from the composite hash. */
        {
            size_t signature_size = OE_EXT_KEY_SIZE;

            if (oe_rsa_private_key_sign(
                    &privkey,
                    OE_HASH_TYPE_SHA256,
                    hash.buf,
                    sizeof(oe_ext_hash_t),
                    signature,
                    &signature_size) != 0)
            {
                OE_RAISE(OE_FAILURE);
            }

            if (signature_size != OE_EXT_KEY_SIZE)
                OE_RAISE(OE_FAILURE);
        }
    }

    /* Initialize the SIGSTRUCT. */
    {
        uint8_t modulus[OE_EXT_KEY_SIZE];
        uint8_t exponent[OE_EXT_EXPONENT_SIZE];

        /* Get the modulus */
        if (_get_modulus(&pubkey, modulus) != 0)
            OE_RAISE(OE_FAILURE);

        /* Get the exponent */
        if (_get_exponent(&pubkey, exponent) != 0)
            OE_RAISE(OE_FAILURE);

        /* sign.signer */
        _compute_hash(&sigstruct->signer, modulus, sizeof(modulus), NULL, 0);

        /* sign.extid*/
        sigstruct->extid = *extid;

        /* sign.extmeasure */
        sigstruct->extmeasure = *extmeasure;

        /* sign.signature */
        memcpy(&sigstruct->signature, signature, sizeof signature);
    }

    result = OE_OK;

done:

    if (data)
        free(data);

    if (privkey_initialized)
        oe_rsa_private_key_free(&privkey);

    if (pubkey_initialized)
        oe_rsa_public_key_free(&pubkey);

    return result;
}
