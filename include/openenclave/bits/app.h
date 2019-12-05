// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_BITS_APP_H
#define _OE_BITS_APP_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/result.h>
#include <openenclave/bits/types.h>

#define OE_APP_SECTION __attribute__((section(".oeapp")))

#define OE_APP_HASH_SIZE 32

#define OE_APP_KEY_SIZE 384

#define OE_APP_MODULUS_SIZE 384

#define OE_APP_EXPONENT_SIZE 4

typedef struct _oe_app_hash
{
    /* 256-bit hash */
    uint8_t buf[OE_APP_HASH_SIZE];
} oe_app_hash_t;

/* 3072-bit RSA public key. */
typedef struct _oe_app_pubkey
{
    /* 3072-bit modulus. */
    uint8_t modulus[OE_APP_KEY_SIZE];

    /* 32-bit exponent . */
    uint8_t exponent[OE_APP_EXPONENT_SIZE];
} oe_app_pubkey_t;

/* A policy updated by 'oeapp update' tool. */
typedef struct _oe_app_policy
{
    /* The public key of the signer. */
    oe_app_pubkey_t pubkey;

    /* The signer's ID (the SHA-256 public signing key) */
    oe_app_hash_t signer;

    /* The hash of the user-defined application structure (APPSTRUCT) */
    oe_app_hash_t appid;

} oe_app_policy_t;

typedef struct _oe_app_signature
{
    uint8_t buf[OE_APP_KEY_SIZE];
} oe_app_signature_t;

/* An signature generated by 'oeapp sign' tool. */
typedef struct _oe_app_sigstruct
{
    /* The signer's ID (the SHA-256 of the public signing key). */
    oe_app_hash_t signer;

    /* The hash of the user-defined application structure (APPSTRUCT) */
    oe_app_hash_t appid;

    /* The hash of the application. */
    oe_app_hash_t apphash;

    /* The signature of hash(apphash|appid). */
    oe_app_signature_t signature;
} oe_app_sigstruct_t;

oe_result_t oe_app_verify_signature(
    oe_app_pubkey_t* pubkey,
    const oe_app_hash_t* appid,
    const oe_app_hash_t* apphash,
    const oe_app_signature_t* signature);

oe_result_t oe_app_ascii_to_hash(const char* ascii, oe_app_hash_t* hash);

void oe_app_dump_hash(const char* name, const oe_app_hash_t* hash);

void oe_app_dump_policy(const oe_app_policy_t* policy);

void oe_app_dump_sigstruct(const oe_app_sigstruct_t* sigstruct);

oe_result_t oe_app_save_sigstruct(
    const char* path,
    const oe_app_sigstruct_t* sigstruct);

oe_result_t oe_app_load_sigstruct(
    const char* path,
    oe_app_sigstruct_t* signature);

#endif /* _OE_BITS_APP_H */
