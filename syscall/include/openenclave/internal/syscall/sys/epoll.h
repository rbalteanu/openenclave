// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYS_EPOLL_H
#define _OE_SYS_EPOLL_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>
#include <openenclave/internal/syscall/sys/bits/sigset.h>
#include <openenclave/internal/types.h>
#include <sys/epoll.h>

OE_EXTERNC_BEGIN

#define __OE_EPOLL_DATA oe_epoll_data
#define __OE_EPOLL_DATA_T oe_epoll_data_t
#include <openenclave/internal/syscall/sys/bits/epoll_data.h>
#undef __OE_EPOLL_DATA
#undef __OE_EPOLL_DATA_T

#define __OE_EPOLL_EVENT oe_epoll_event
#include <openenclave/internal/syscall/sys/bits/epoll_event.h>
#undef __OE_EPOLL_EVENT

int oe_epoll_create(int size);

int oe_epoll_create1(int flags);

int oe_epoll_ctl(int epfd, int op, int fd, struct oe_epoll_event* event);

int oe_epoll_wait(
    int epfd,
    struct oe_epoll_event* events,
    int maxevents,
    int timeout);

int oe_epoll_pwait(
    int epfd,
    struct oe_epoll_event* events,
    int maxevents,
    int timeout,
    const oe_sigset_t* sigmask);

OE_EXTERNC_END

#endif /* _OE_SYS_EPOLL_H */
