/* Copyright (c) Open Enclave SDK contributors. */
/* Licensed under the MIT License. */

#ifndef _OE_SYSCALL_NETINET_IN_H
#define _OE_SYSCALL_NETINET_IN_H

#include <netinet/in.h>
#include <openenclave/bits/defs.h>
#include <openenclave/internal/syscall/bits/types.h>

OE_EXTERNC_BEGIN

// clang-format off
#define OE_IN6ADDR_ANY_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } }
#define OE_IN6ADDR_LOOPBACK_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }
// clang-format on

struct oe_in_addr
{
    oe_in_addr_t s_addr;
};

#define __OE_IN6_ADDR oe_in6_addr
#include <openenclave/internal/syscall/netinet/bits/in6_addr.h>
#undef __OE_IN6_ADDR

#define oe_s6_addr __in6_union.__s6_addr
#define oe_s6_addr16 __in6_union.__s6_addr16
#define oe_s6_addr32 __in6_union.__s6_addr32

#define __OE_SOCKADDR_IN oe_sockaddr_in
#include <openenclave/internal/syscall/netinet/bits/sockaddr_in.h>
#undef __OE_SOCKADDR_IN

#define __OE_SOCKADDR_IN6 oe_sockaddr_in6
#include <openenclave/internal/syscall/netinet/bits/sockaddr_in6.h>
#undef __OE_SOCKADDR_IN6

OE_EXTERNC_END

#endif /* _OE_SYSCALL_NETINET_IN_H */
