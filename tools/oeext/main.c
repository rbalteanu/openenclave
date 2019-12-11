// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <openenclave/bits/defs.h>
#include <openenclave/bits/ext.h>
#include <openenclave/internal/elf.h>
#include <openenclave/internal/files.h>
#include <openenclave/internal/raise.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../../host/crypto/rsa.h"

static const char* arg0;

OE_PRINTF_FORMAT(1, 2)
static void _err(const char* format, ...)
{
    fprintf(stderr, "\n");

    fprintf(stderr, "%s: error: ", arg0);

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fprintf(stderr, "\n\n");

    exit(1);
}

static bool _valid_symbol_name(const char* name)
{
    bool ret = false;
    const char* p = name;

    if (*p != '_' && !isalpha(*p))
        goto done;

    p++;

    while (*p == '_' || isalnum(*p))
        p++;

    if (*p != '\0')
        goto done;

    ret = true;

done:
    return ret;
}

static uint64_t _find_file_offset(elf64_t* elf, uint64_t vaddr)
{
    elf64_ehdr_t* eh = (elf64_ehdr_t*)elf->data;
    elf64_phdr_t* ph = (elf64_phdr_t*)((uint8_t*)elf->data + eh->e_phoff);
    size_t i;

    /* Search for the segment that contains this virtual address. */
    for (i = 0; i < eh->e_phnum; i++)
    {
        if (vaddr >= ph->p_vaddr && vaddr < ph->p_vaddr + ph->p_memsz)
        {
            size_t vaddr_offset = vaddr - ph->p_vaddr;

            /* Calculate the offset within the file. */
            size_t file_offset = ph->p_offset + vaddr_offset;

            if (file_offset >= elf->size)
                return (uint64_t)-1;

            return file_offset;
        }

        ph++;
    }

    return (uint64_t)-1;
}

static int _get_opt(
    int* argc,
    const char* argv[],
    const char* name,
    const char** opt)
{
    size_t len = strlen(name);

    for (int i = 0; i < *argc; i++)
    {
        if (strncmp(argv[i], name, len) == 0 && argv[i][len] == '=')
        {
            *opt = &argv[i][len + 1];
            size_t n = (size_t)(*argc - i) * sizeof(char*);
            memmove(&argv[i], &argv[i + 1], n);
            (*argc)--;
            return 0;
        }
    }

    /* Not found */
    return -1;
}

static int _register_main(int argc, const char* argv[])
{
    int ret = 1;
    static const char _usage[] =
        "\n"
        "Usage: %s register enclave=? symbol=? pubkey=? extid=? [payload=?]\n"
        "\n";
    const char* pubkey;
    oe_ext_hash_t extid;
    const char* enclave;
    const char* symbol;
    const char* payload;

    /* Check and collect arguments. */
    if (argc < 6)
    {
        fprintf(stderr, _usage, arg0);
        goto done;
    }

    /* Collect the options. */
    {
        /* Handle pubkey option. */
        if (_get_opt(&argc, argv, "pubkey", &pubkey) != 0)
            _err("missing pubkey option");

        /* Get the extid option. */
        {
            const char* ascii;

            if (_get_opt(&argc, argv, "extid", &ascii) != 0)
                _err("missing 'extid' option");

            if (oe_ext_ascii_to_hash(ascii, &extid) != OE_OK)
                _err("bad 'extid' option: %s", ascii);
        }

        /* Handle enclave option. */
        if (_get_opt(&argc, argv, "enclave", &enclave) != 0)
            _err("missing 'enclave' option");

        /* Get symbol option. */
        {
            if (_get_opt(&argc, argv, "symbol", &symbol) != 0)
                _err("missing symbol option");

            if (!_valid_symbol_name(symbol))
                _err("bad value for symbol option: %s", symbol);
        }

        /* Get optional payload option. */
        _get_opt(&argc, argv, "payload", &payload);
    }

    if (oe_ext_register(enclave, symbol, pubkey, &extid, payload) != OE_OK)
    {
        _err("registration failed");
    }

    ret = 0;

done:

    return ret;
}

static int _dump_main(int argc, const char* argv[])
{
    static const char _usage[] = "\n"
                                 "Usage: %s dump enclave=? symbol=?\n"
                                 "\n";
    const char* enclave;
    const char* symbol;
    elf64_t elf;
    bool loaded = false;
    elf64_sym_t sym;
    uint8_t* symbol_address;
    size_t file_offset;

    int ret = 1;

    /* Check and collect arguments. */
    if (argc != 4)
    {
        fprintf(stderr, _usage, arg0);
        goto done;
    }

    /* Collect the options. */
    {
        /* Handle enclave option. */
        if (_get_opt(&argc, argv, "enclave", &enclave) != 0)
            _err("missing enclave option");

        /* Get symbol option. */
        {
            if (_get_opt(&argc, argv, "symbol", &symbol) != 0)
                _err("missing symbol option");

            if (!_valid_symbol_name(symbol))
                _err("bad value for symbol option: %s", symbol);
        }
    }

    /* Load the ELF-64 object */
    {
        if (elf64_load(enclave, &elf) != 0)
            _err("cannot load %s", enclave);

        loaded = true;
    }

    /* Find the symbol within the ELF image. */
    if (elf64_find_symbol_by_name(&elf, symbol, &sym) != 0)
        _err("cannot find symbol: %s", symbol);

    /* Check the size of the symbol. */
    if (sym.st_size != sizeof(oe_ext_registration_t))
        _err("symbol %s is wrong size", symbol);

    /* Find the offset within the ELF file of this symbol. */
    if ((file_offset = _find_file_offset(&elf, sym.st_value)) == (uint64_t)-1)
        _err("cannot locate symbol %s in %s", symbol, enclave);

    /* Make sure the entire symbol falls within the file image. */
    if (file_offset + sizeof(oe_ext_registration_t) >= elf.size)
        _err("unexpected");

    /* Get the address of the symbol. */
    symbol_address = (uint8_t*)elf.data + file_offset;

    /* Print the 'registration' symbol. */
    {
        oe_ext_registration_t registration;

        /* Update the registration structure in the ELF file. */
        memcpy(&registration, symbol_address, sizeof(registration));

        oe_ext_dump_registration(&registration);
    }

    ret = 0;

done:

    if (loaded)
        elf64_unload(&elf);

    return ret;
}

static int _sign_main(int argc, const char* argv[])
{
    static const char _usage[] =
        "\n"
        "Usage: %s sign privkey=? extid=? extmeasure=? sigstructfile=?\n"
        "\n";
    const char* privkey;
    oe_ext_hash_t extid;
    oe_ext_hash_t extmeasure;
    const char* sigstructfile;
    oe_ext_sigstruct_t sigstruct;

    int ret = 1;

    /* Check usage. */
    if (argc != 6)
    {
        fprintf(stderr, _usage, arg0);
        goto done;
    }

    /* Collect the options. */
    {
        /* Get pubkey option. */
        if (_get_opt(&argc, argv, "privkey", &privkey) != 0)
            _err("missing privkey option");

        /* Get the extid option. */
        {
            const char* ascii;

            if (_get_opt(&argc, argv, "extid", &ascii) != 0)
                _err("missing extid option");

            if (oe_ext_ascii_to_hash(ascii, &extid) != OE_OK)
                _err("bad extid option: %s", ascii);
        }

        /* Get the extmeasure option. */
        {
            const char* ascii;

            if (_get_opt(&argc, argv, "extmeasure", &ascii) != 0)
                _err("missing extmeasure option");

            if (oe_ext_ascii_to_hash(ascii, &extmeasure) != OE_OK)
                _err("bad extmeasure option: %s", ascii);
        }

        /* Get the sigstructfile option. */
        if (_get_opt(&argc, argv, "sigstructfile", &sigstructfile) != 0)
            _err("missing sigstructfile option");
    }

    /* Perform the signing operation. */
    if (oe_ext_sign(privkey, &extid, &extmeasure, &sigstruct) != OE_OK)
        _err("signing operation failed");

    /* Save the sigstruct to a file. */
    if (oe_ext_save_sigstruct(sigstructfile, &sigstruct) != OE_OK)
        _err("failed to save: %s", sigstructfile);

    ret = 0;

done:

    return ret;
}

int main(int argc, const char* argv[])
{
    static const char _usage[] =
        "\n"
        "Usage: %s command options...\n"
        "\n"
        "Commands:\n"
        "    register - update the registration in an enclave.\n"
        "    sign - create a sigstruct file for a given signer and hash.\n"
        "    dump - dump an enclave update sructure.\n"
        "\n";
    int ret = 1;

    arg0 = argv[0];

    if (argc < 2)
    {
        fprintf(stderr, _usage, argv[0]);
        goto done;
    }

    /* Disable logging noise to standard output. */
    setenv("OE_LOG_LEVEL", "NONE", 1);

    if (strcmp(argv[1], "register") == 0)
    {
        ret = _register_main(argc, argv);
        goto done;
    }
    else if (strcmp(argv[1], "sign") == 0)
    {
        ret = _sign_main(argc, argv);
        goto done;
    }
    if (strcmp(argv[1], "dump") == 0)
    {
        ret = _dump_main(argc, argv);
        goto done;
    }
    else
    {
        _err("unknown subcommand: %s", argv[1]);
    }

done:
    return ret;
}
