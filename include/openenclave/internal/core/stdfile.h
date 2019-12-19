// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_INTERNAL_CORE_STDFILE_H
#define _OE_INTERNAL_CORE_STDFILE_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

typedef struct _OE_IO_FILE OE_FILE;
extern OE_FILE* const oe_stdin;
extern OE_FILE* const oe_stdout;
extern OE_FILE* const oe_stderr;

OE_EXTERNC_END

#endif /* _OE_INTERNAL_CORE_STDFILE_H */
