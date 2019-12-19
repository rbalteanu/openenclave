// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

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

static long _dispatch_oe_syscall(
    long n,
    long x1,
    long x2,
    long x3,
    long x4,
    long x5,
    long x6)
{
    long ret;

    switch (n)
    {
#if defined(SYS_stat)
        case SYS_stat:
        {
            struct stat* stat = (struct stat*)x2;
            struct oe_stat oe_stat;

            _stat_to_oe_stat(stat, &oe_stat);
            x2 = (long)&oe_stat;
            ret = oe_syscall(OE_SYS_stat, x1, x2, x3, x4, x5, x6);
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
            ret = oe_syscall(OE_SYS_newfstatat, x1, x2, x3, x4, x5, x6);
            _oe_stat_to_stat(&oe_stat, stat);

            break;
        }
        default:
        {
            ret = oe_syscall(n, x1, x2, x3, x4, x5, x6);
            break;
        }
    }

    return ret;
}
