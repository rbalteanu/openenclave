// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_NETDB_H_
#define _OE_SYSCALL_NETDB_H_

#include <netdb.h>
#include <openenclave/internal/syscall/sys/socket.h>

OE_EXTERNC_BEGIN

#define __OE_ADDRINFO oe_addrinfo
#define __OE_SOCKADDR oe_sockaddr
#include <openenclave/internal/syscall/bits/addrinfo.h>
#undef __OE_ADDRINFO
#undef __OE_SOCKADDR

int oe_getaddrinfo(
    const char* node,
    const char* service,
    const struct oe_addrinfo* hints,
    struct oe_addrinfo** res);

void oe_freeaddrinfo(struct oe_addrinfo* res);

int oe_getnameinfo(
    const struct oe_sockaddr* sa,
    oe_socklen_t salen,
    char* host,
    oe_socklen_t hostlen,
    char* serv,
    oe_socklen_t servlen,
    int flags);

OE_EXTERNC_END

#endif /* netinet/netdb.h */
