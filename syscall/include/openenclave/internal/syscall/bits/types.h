// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_BITS_TYPES_H
#define _OE_SYSCALL_BITS_TYPES_H

#include <openenclave/bits/types.h>

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

OE_EXTERNC_END

#endif // _OE_SYSCALL_BITS_TYPES_H
