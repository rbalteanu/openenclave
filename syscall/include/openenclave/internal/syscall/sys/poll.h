// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_SYS_POLL_H
#define _OE_SYSCALL_SYS_POLL_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

typedef uint64_t oe_nfds_t;

struct oe_pollfd
{
    int fd;            /* File descriptor to poll.  */
    short int events;  /* Types of events poller cares about.  */
    short int revents; /* Types of events that actually occurred.  */
};

int oe_poll(struct oe_pollfd* fds, oe_nfds_t nfds, int timeout);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_SYS_POLL_H */
