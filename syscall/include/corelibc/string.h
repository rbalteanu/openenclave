// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_STRING_H
#define _OE_STRING_H

#include <openenclave/bits/types.h>
#include <openenclave/corelibc/bits/defs.h>
#include <openenclave/internal/core/string.h>

OE_EXTERNC_BEGIN

#if defined(OE_NEED_STDC_NAMES)

OE_INLINE
size_t strlen(const char* s)
{
    return oe_strlen(s);
}

OE_INLINE
int strcmp(const char* s1, const char* s2)
{
    return oe_strcmp(s1, s2);
}

OE_INLINE
int strncmp(const char* s1, const char* s2, size_t n)
{
    return oe_strncmp(s1, s2, n);
}

OE_INLINE
char* strstr(const char* haystack, const char* needle)
{
    return oe_strstr(haystack, needle);
}

OE_INLINE
size_t strlcpy(char* dest, const char* src, size_t size)
{
    return oe_strlcpy(dest, src, size);
}

OE_INLINE
size_t strlcat(char* dest, const char* src, size_t size)
{
    return oe_strlcat(dest, src, size);
}

OE_INLINE
char* strerror(int errnum)
{
    return oe_strerror(errnum);
}

OE_INLINE
int strerror_r(int errnum, char* buf, size_t buflen)
{
    return oe_strerror_r(errnum, buf, buflen);
}

OE_INLINE
char* strtok_r(char* str, const char* delim, char** saveptr)
{
    return oe_strtok_r(str, delim, saveptr);
}

OE_INLINE char* strdup(const char* s)
{
    return oe_strdup(s);
}

OE_INLINE size_t strspn(const char* s, const char* accept)
{
    return oe_strspn(s, accept);
}

OE_INLINE size_t strcspn(const char* s, const char* reject)
{
    return oe_strcspn(s, reject);
}

OE_INLINE char* strchr(const char* s, int c)
{
    return oe_strchr(s, c);
}

OE_INLINE char* strchrnul(const char* s, int c)
{
    return oe_strchrnul(s, c);
}

OE_INLINE char* strrchr(const char* s, int c)
{
    return oe_strrchr(s, c);
}

#endif /* defined(OE_NEED_STDC_NAMES) */

OE_EXTERNC_END

#endif /* _OE_STRING_H */
