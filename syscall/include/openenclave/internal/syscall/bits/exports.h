// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_INTERNAL_EXPORTS_H
#define _OE_SYSCALL_INTERNAL_EXPORTS_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

/* The syscall layer depends on these non-exported OE symbols. */

OE_EXTERNC_BEGIN

int memcmp(const void* vl, const void* vr, size_t n);

void* memcpy(void* dest, const void* src, size_t n);

void* memmove(void* dest, const void* src, size_t n);

void* memset(void* dest, int c, size_t n);

int oe_strcmp(const char* s1, const char* s2);

int oe_strncmp(const char* s1, const char* s2, size_t n);

size_t oe_strlen(const char* s);

char* oe_strdup(const char* s);

size_t oe_strlcpy(char* dest, const char* src, size_t size);

size_t oe_strlcat(char* dest, const char* src, size_t size);

char* oe_strtok_r(char* str, const char* delim, char** saveptr);

void* oe_malloc(size_t size);

void oe_free(void* ptr);

void* oe_calloc(size_t nmemb, size_t size);

void* oe_realloc(void* ptr, size_t size);

void oe_abort(void);

int oe_atexit(void (*function)(void));

int oe_host_printf(const char* fmt, ...);

extern int* __oe_errno_location(void);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_INTERNAL_EXPORTS_H */
