// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_SYS_STAT_H
#define _OE_SYSCALL_SYS_STAT_H

#include <openenclave/internal/syscall/bits/defs.h>
#include <openenclave/internal/syscall/bits/types.h>
#include <sys/stat.h>

OE_EXTERNC_BEGIN

#define __OE_STAT oe_stat
#include <openenclave/internal/syscall/sys/bits/stat.h>
#undef __OE_STAT

#ifndef st_atime
#define st_atime st_atim.tv_sec
#endif

#ifndef st_ctime
#define st_mtime st_mtim.tv_sec
#endif

#ifndef st_ctime
#define st_ctime st_ctim.tv_sec
#endif

int oe_stat(const char* pathname, struct oe_stat* buf);

int oe_stat_d(uint64_t devid, const char* pathname, struct oe_stat* buf);

int oe_mkdir(const char* pathname, oe_mode_t mode);

int oe_mkdir_d(uint64_t devid, const char* pathname, oe_mode_t mode);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_SYS_STAT_H */
