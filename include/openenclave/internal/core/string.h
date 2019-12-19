// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_INTERNAL_CORE_STRING_H
#define _OE_INTERNAL_CORE_STRING_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

/* The mem methods are always defined by their stdc names in oecore */
int memcmp(const void* vl, const void* vr, size_t n);
void* memcpy(void* OE_RESTRICT dest, const void* OE_RESTRICT src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* dest, int c, size_t n);

size_t oe_strlen(const char* s);

int oe_strcmp(const char* s1, const char* s2);

int oe_strncmp(const char* s1, const char* s2, size_t n);

size_t oe_strlcpy(char* dest, const char* src, size_t size);

size_t oe_strlcat(char* dest, const char* src, size_t size);

char* oe_strerror(int errnum);

int oe_strerror_r(int errnum, char* buf, size_t buflen);

OE_EXTERNC_END

#endif /* _OE_INTERNAL_CORE_STRING_H */
