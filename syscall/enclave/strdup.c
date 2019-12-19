// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <syscall/common.h>
#include <syscall/string.h>

char* oe_strdup(const char* s)
{
    char* p;
    size_t n;

    if (!s)
        return NULL;

    n = oe_strlen(s) + 1;

    if (!(p = oe_malloc(n)))
        return NULL;

    memcpy(p, s, n);

    return p;
}
