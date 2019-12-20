// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_STRING_H
#define _OE_SYSCALL_STRING_H

#include <openenclave/internal/core/string.h>
#include <syscall/common.h>

OE_EXTERNC_BEGIN

char* oe_strdup(const char* s);

char* oe_strtok_r(char* s, const char* delim, char** lasts);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_STRING_H */
