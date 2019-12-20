// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#define strerror musl_strerror
#define strerror_l musl_strerror_l
#include "../3rdparty/musl/musl/src/errno/strerror.c"
#undef strerror
#undef strerror_l

char* strerror(int e)
{
    char* s;

    if (!(s = musl_strerror(e)))
        return NULL;

    /* The libcxx test suite expects "Unknown error" instead. */
    if (strcmp(s, "No error information") == 0)
        return "Unknown error";

    return s;
}
