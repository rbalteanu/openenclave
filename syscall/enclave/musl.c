// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

/* Integration with MUSL C library */

#include <openenclave/internal/syscall.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <syscall/common.h>
#include <syscall/module.h>
#include <syscall/sys/stat.h>
#include <syscall/sys/syscall.h>

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#include <netdb.h>
#include <syscall/netdb.h>

static void _stat_to_oe_stat(struct stat* stat, struct oe_stat* oe_stat)
{
    oe_stat->st_dev = stat->st_dev;
    oe_stat->st_ino = stat->st_ino;
    oe_stat->st_nlink = stat->st_nlink;
    oe_stat->st_mode = stat->st_mode;
    oe_stat->st_uid = stat->st_uid;
    oe_stat->st_gid = stat->st_gid;
    oe_stat->st_rdev = stat->st_rdev;
    oe_stat->st_size = stat->st_size;
    oe_stat->st_blksize = stat->st_blksize;
    oe_stat->st_blocks = stat->st_blocks;
    oe_stat->st_atim.tv_sec = stat->st_atim.tv_sec;
    oe_stat->st_atim.tv_nsec = stat->st_atim.tv_nsec;
    oe_stat->st_ctim.tv_sec = stat->st_ctim.tv_sec;
    oe_stat->st_ctim.tv_nsec = stat->st_ctim.tv_nsec;
    oe_stat->st_mtim.tv_sec = stat->st_mtim.tv_sec;
    oe_stat->st_mtim.tv_nsec = stat->st_mtim.tv_nsec;
}

static void _oe_stat_to_stat(struct oe_stat* oe_stat, struct stat* stat)
{
    stat->st_dev = oe_stat->st_dev;
    stat->st_ino = oe_stat->st_ino;
    stat->st_nlink = oe_stat->st_nlink;
    stat->st_mode = oe_stat->st_mode;
    stat->st_uid = oe_stat->st_uid;
    stat->st_gid = oe_stat->st_gid;
    stat->st_rdev = oe_stat->st_rdev;
    stat->st_size = oe_stat->st_size;
    stat->st_blksize = oe_stat->st_blksize;
    stat->st_blocks = oe_stat->st_blocks;
    stat->st_atim.tv_sec = oe_stat->st_atim.tv_sec;
    stat->st_atim.tv_nsec = oe_stat->st_atim.tv_nsec;
    stat->st_ctim.tv_sec = oe_stat->st_ctim.tv_sec;
    stat->st_ctim.tv_nsec = oe_stat->st_ctim.tv_nsec;
    stat->st_mtim.tv_sec = oe_stat->st_mtim.tv_sec;
    stat->st_mtim.tv_nsec = oe_stat->st_mtim.tv_nsec;
}

static oe_result_t _syscall_hook(
    long n,
    long x1,
    long x2,
    long x3,
    long x4,
    long x5,
    long x6,
    long* ret)
{
    oe_result_t result = OE_UNSUPPORTED;

    switch (n)
    {
#if defined(SYS_stat)
        case SYS_stat:
        {
            struct stat* stat = (struct stat*)x2;
            struct oe_stat oe_stat;

            _stat_to_oe_stat(stat, &oe_stat);
            x2 = (long)&oe_stat;
            *ret = oe_syscall(OE_SYS_stat, x1, x2, x3, x4, x5, x6);

            if (*ret == OE_ENOSYS)
                goto done;

            _oe_stat_to_stat(&oe_stat, stat);
            break;
        }
#endif
        case SYS_newfstatat:
        {
            struct stat* stat = (struct stat*)x3;
            struct oe_stat oe_stat;

            _stat_to_oe_stat(stat, &oe_stat);
            x3 = (long)&oe_stat;
            *ret = oe_syscall(OE_SYS_newfstatat, x1, x2, x3, x4, x5, x6);

            if (*ret == OE_ENOSYS)
                goto done;

            _oe_stat_to_stat(&oe_stat, stat);
            break;
        }
        default:
        {
            if ((*ret = oe_syscall(n, x1, x2, x3, x4, x5, x6)) == OE_ENOSYS)
                goto done;

            break;
        }
    }

    result = OE_OK;

done:
    return result;
}

void freeaddrinfo(struct addrinfo* res)
{
    oe_freeaddrinfo((struct oe_addrinfo*)res);
}

int getaddrinfo(
    const char* node,
    const char* service,
    const struct addrinfo* hints,
    struct addrinfo** res)
{
    return oe_getaddrinfo(
        node,
        service,
        (const struct oe_addrinfo*)hints,
        (struct oe_addrinfo**)res);
}

int getnameinfo(
    const struct sockaddr* sa,
    socklen_t salen,
    char* host,
    socklen_t hostlen,
    char* serv,
    socklen_t servlen,
    int flags)
{
    return oe_getnameinfo(
        (struct oe_sockaddr*)sa, salen, host, hostlen, serv, servlen, flags);
}

oe_result_t oe_load_module_syscall(void)
{
    oe_register_syscall_hook(_syscall_hook);
    return OE_OK;
}
