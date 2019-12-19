// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/internal/core/malloc.h>

void* malloc(size_t size)
{
    return oe_malloc(size);
}

void free(void* ptr)
{
    return oe_free(ptr);
}

void* calloc(size_t nmemb, size_t size)
{
    return oe_calloc(nmemb, size);
}

void* realloc(void* ptr, size_t size)
{
    return oe_realloc(ptr, size);
}

void memalign_free(void* ptr)
{
    return oe_memalign_free(ptr);
}

void* _memalign(size_t alignment, size_t size)
{
    return oe_memalign(alignment, size);
}

int posix_memalign(void** memptr, size_t alignment, size_t size)
{
    return oe_posix_memalign(memptr, alignment, size);
}
