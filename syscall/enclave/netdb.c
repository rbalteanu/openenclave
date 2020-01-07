// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/internal/syscall/bits/exports.h>
#include <openenclave/internal/syscall/netdb.h>
#include <openenclave/internal/syscall/raise.h>
#include <openenclave/internal/syscall/resolver.h>
#include <openenclave/internal/syscall/stdlib.h>
#include <openenclave/internal/syscall/sys/socket.h>
#include <pthread.h>
#include <stdlib.h>

static oe_resolver_t* _resolver;
static pthread_spinlock_t _lock;
static bool _installed_atexit_handler = false;

static __attribute__((constructor)) void _init_lock(void)
{
    pthread_spin_init(&_lock, PTHREAD_PROCESS_PRIVATE);
}

static void _atexit_handler(void)
{
    if (_resolver)
        _resolver->ops->release(_resolver);
}

/* Called by the public oe_load_module_host_resolver() function. */
int oe_register_resolver(oe_resolver_t* resolver)
{
    int ret = -1;
    bool locked = false;

    /* Check parameters. */
    if (!resolver || !resolver->ops || !resolver->ops->getaddrinfo ||
        !resolver->ops->getnameinfo)
    {
        OE_RAISE_ERRNO(EINVAL);
    }

    pthread_spin_lock(&_lock);
    locked = true;

    /* This function can be called only once. */
    if (_resolver != NULL)
        OE_RAISE_ERRNO(EINVAL);

    _resolver = resolver;

    if (!_installed_atexit_handler)
    {
        atexit(_atexit_handler);
        _installed_atexit_handler = true;
    }

    ret = 0;

done:

    if (locked)
        pthread_spin_unlock(&_lock);

    return ret;
}

int oe_getaddrinfo(
    const char* node,
    const char* service,
    const struct oe_addrinfo* hints,
    struct oe_addrinfo** res_out)
{
    int ret = EAI_FAIL;
    struct oe_addrinfo* res;
    bool locked = false;

    if (res_out)
        *res_out = NULL;
    else
        OE_RAISE_ERRNO(EINVAL);

    pthread_spin_lock(&_lock);
    locked = true;

    if (!_resolver)
    {
        ret = EAI_SYSTEM;
        OE_RAISE_ERRNO(EINVAL);
    }

    ret = (_resolver->ops->getaddrinfo)(_resolver, node, service, hints, &res);

    if (ret == 0)
        *res_out = res;

done:

    if (locked)
        pthread_spin_unlock(&_lock);

    return ret;
}

int oe_getnameinfo(
    const struct oe_sockaddr* sa,
    socklen_t salen,
    char* host,
    socklen_t hostlen,
    char* serv,
    socklen_t servlen,
    int flags)
{
    ssize_t ret = EAI_FAIL;
    bool locked = false;

    pthread_spin_lock(&_lock);
    locked = true;

    if (!_resolver)
    {
        ret = EAI_SYSTEM;
        OE_RAISE_ERRNO(EINVAL);
    }

    ret = (*_resolver->ops->getnameinfo)(
        _resolver, sa, salen, host, hostlen, serv, servlen, flags);

done:

    if (locked)
        pthread_spin_unlock(&_lock);

    return (int)ret;
}

void oe_freeaddrinfo(struct oe_addrinfo* res)
{
    struct oe_addrinfo* p;

    for (p = res; p;)
    {
        struct oe_addrinfo* next = p->ai_next;

        free(p->ai_addr);
        free(p->ai_canonname);
        free(p);

        p = next;
    }
}
