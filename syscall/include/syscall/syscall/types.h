// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_TYPES_H
#define _OE_SYSCALL_TYPES_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

typedef int64_t oe_intptr_t;
typedef uint32_t oe_mode_t;
typedef int64_t oe_off_t;
typedef uint64_t oe_ino_t;
typedef uint64_t oe_dev_t;
typedef uint32_t oe_gid_t;
typedef uint32_t oe_uid_t;
typedef int oe_pid_t;
typedef uint64_t oe_nlink_t;
typedef int64_t oe_blksize_t;
typedef int64_t oe_blkcnt_t;
typedef int64_t oe_time_t;
typedef int64_t oe_suseconds_t;
typedef uint32_t oe_socklen_t;
typedef uint16_t oe_sa_family_t;
typedef uint16_t oe_in_port_t;
typedef uint32_t oe_in_addr_t;
typedef struct _OE_DIR OE_DIR;
struct oe_dirent;

typedef int64_t oe_host_fd_t;

/* Version of struct oe_pollfd with wider descriptor. */
struct oe_host_pollfd
{
    oe_host_fd_t fd;
    short int events;
    short int revents;
};

struct oe_timespec
{
    time_t tv_sec;
    long tv_nsec;
};

OE_STATIC_ASSERT(sizeof(struct oe_host_pollfd) == (2 * sizeof(uint64_t)));
OE_STATIC_ASSERT(OE_OFFSETOF(struct oe_host_pollfd, fd) == 0);
OE_STATIC_ASSERT(OE_OFFSETOF(struct oe_host_pollfd, events) == 8);
OE_STATIC_ASSERT(OE_OFFSETOF(struct oe_host_pollfd, revents) == 10);

OE_EXTERNC_END

#endif // _OE_SYSCALL_TYPES_H
