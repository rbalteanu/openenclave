// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_DIRENT_H
#define _OE_SYSCALL_DIRENT_H

#include <limits.h>
#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

typedef struct _OE_DIR OE_DIR;

#define __OE_DIRENT oe_dirent
#include <openenclave/internal/syscall/bits/dirent.h>
#undef __OE_DIRENT

OE_DIR* oe_opendir(const char* pathname);

OE_DIR* oe_opendir_d(uint64_t devid, const char* pathname);

struct oe_dirent* oe_readdir(OE_DIR* dir);

void oe_rewinddir(OE_DIR* dir);

int oe_closedir(OE_DIR* dir);

int oe_getdents64(unsigned int fd, struct oe_dirent* dirp, unsigned int count);

OE_EXTERNC_END

#endif /* _OE_SYSCALL_DIRENT_H */
