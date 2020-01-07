// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

// clang-format off
#include <openenclave/enclave.h>
#include <openenclave/internal/syscall/bits/exports.h>
// clang-format on

#include <openenclave/internal/syscall/stdlib.h>
#include <openenclave/internal/syscall/stdio.h>
#include <openenclave/internal/syscall/device.h>
#include <openenclave/internal/syscall/raise.h>
#include <openenclave/bits/safecrt.h>
#include <openenclave/internal/syscall/bits/exports.h>
#include <limits.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MOUNT_TABLE_SIZE 64

typedef struct _mount_point
{
    char* path;
    size_t path_size;
    oe_device_t* fs;
    uint32_t flags;
} mount_point_t;

static mount_point_t _mount_table[MAX_MOUNT_TABLE_SIZE];
size_t _mount_table_size = 0;
static pthread_spinlock_t _lock;

static __attribute__((constructor)) void _init_lock(void)
{
    pthread_spin_init(&_lock, PTHREAD_PROCESS_PRIVATE);
}

static bool _installed_free_mount_table = false;

static void _free_mount_table(void)
{
    for (size_t i = 0; i < _mount_table_size; i++)
        free(_mount_table[i].path);
}

oe_device_t* oe_mount_resolve(const char* path, char suffix[OE_PATH_MAX])
{
    oe_device_t* ret = NULL;
    size_t match_len = 0;
    oe_syscall_path_t realpath;
    bool locked = false;

    if (!path || !suffix)
        OE_RAISE_ERRNO(EINVAL);

    /* First check whether a device id is set for this thread. */
    {
        uint64_t devid;

        if ((devid = oe_get_thread_devid()) != OE_DEVID_NONE)
        {
            oe_device_t* device;

            if (!(device =
                      oe_device_table_get(devid, OE_DEVICE_TYPE_FILE_SYSTEM)))
            {
                OE_RAISE_ERRNO(EINVAL);
            }

            /* Use this device. */
            if (strlcpy(suffix, path, OE_PATH_MAX) >= OE_PATH_MAX)
                OE_RAISE_ERRNO(ENAMETOOLONG);

            ret = device;
            goto done;
        }
    }

    /* Find the real path (the absolute non-relative path). */
    if (!oe_realpath(path, &realpath))
        OE_RAISE_ERRNO(errno);

    pthread_spin_lock(&_lock);
    locked = true;

    /* Find the longest binding point that contains this path. */
    for (size_t i = 0; i < _mount_table_size; i++)
    {
        size_t len = strlen(_mount_table[i].path);
        const char* mpath = _mount_table[i].path;

        if (mpath[0] == '/' && mpath[1] == '\0')
        {
            if (len > match_len)
            {
                strlcpy(suffix, realpath.buf, OE_PATH_MAX);
                match_len = len;
                ret = _mount_table[i].fs;
            }
        }
        else if (
            strncmp(mpath, realpath.buf, len) == 0 &&
            (realpath.buf[len] == '/' || realpath.buf[len] == '\0'))
        {
            if (len > match_len)
            {
                strlcpy(suffix, realpath.buf + len, OE_PATH_MAX);

                if (*suffix == '\0')
                    strlcpy(suffix, "/", OE_PATH_MAX);

                match_len = len;
                ret = _mount_table[i].fs;
            }
        }
    }

    if (locked)
    {
        pthread_spin_unlock(&_lock);
        locked = false;
    }

    if (!ret)
        OE_RAISE_ERRNO_MSG(ENOENT, "path=%s", path);

done:

    if (locked)
        pthread_spin_unlock(&_lock);

    return ret;
}

int oe_mount(
    const char* source,
    const char* target,
    const char* filesystemtype,
    unsigned long mountflags,
    const void* data)
{
    int ret = -1;
    oe_device_t* device = NULL;
    oe_device_t* new_device = NULL;
    bool locked = false;
    oe_syscall_path_t source_path;
    oe_syscall_path_t target_path;
    mount_point_t mount_point = {0};

    if (!target || !filesystemtype)
        OE_RAISE_ERRNO(EINVAL);

    /* Normalize the target path. */
    {
        if (!oe_realpath(target, &target_path))
            OE_RAISE_ERRNO(EINVAL);

        target = target_path.buf;
    }

    /* Normalize the source path if any. */
    if (source)
    {
        if (!oe_realpath(source, &source_path))
            OE_RAISE_ERRNO(EINVAL);

        source = source_path.buf;
    }

    /* Resolve the device for the given filesystemtype. */
    device = oe_device_table_find(filesystemtype, OE_DEVICE_TYPE_FILE_SYSTEM);
    if (!device)
        OE_RAISE_ERRNO_MSG(ENODEV, "filesystemtype=%s", filesystemtype);

    /* Be sure the full_target directory exists (if not root). */
    if (strcmp(target, "/") != 0)
    {
        struct oe_stat buf;
        int retval = -1;

        if ((retval = oe_stat(target, &buf)) != 0)
            OE_RAISE_ERRNO(errno);

        if (!OE_S_ISDIR(buf.st_mode))
            OE_RAISE_ERRNO(ENOTDIR);
    }

    /* Lock the mount table. */
    pthread_spin_lock(&_lock);
    locked = true;

    /* Install _free_mount_table() if not already installed. */
    if (_installed_free_mount_table == false)
    {
        atexit(_free_mount_table);
        _installed_free_mount_table = true;
    }

    /* Fail if mount table exhausted. */
    if (_mount_table_size == MAX_MOUNT_TABLE_SIZE)
        OE_RAISE_ERRNO(ENOMEM);

    /* Reject duplicate mount paths. */
    for (size_t i = 0; i < _mount_table_size; i++)
    {
        if (strcmp(_mount_table[i].path, target) == 0)
            OE_RAISE_ERRNO(EEXIST);
    }

    /* Clone the device. */
    if (device->ops.fs.clone(device, &new_device) != 0)
        OE_RAISE_ERRNO(errno);

    /* Assign and initialize new mount point. */
    {
        if (!(mount_point.path = strdup(target)))
            OE_RAISE_ERRNO(ENOMEM);

        mount_point.path_size = strlen(target) + 1;
        mount_point.fs = new_device;
        mount_point.flags = 0;
    }

    /* Notify the device that it has been mounted. */
    if (new_device->ops.fs.mount(
            new_device, source, target, filesystemtype, mountflags, data) != 0)
    {
        goto done;
    }

    _mount_table[_mount_table_size++] = mount_point;
    new_device = NULL;
    mount_point.path = NULL;
    ret = 0;

done:

    if (mount_point.path)
        free(mount_point.path);

    if (locked)
        pthread_spin_unlock(&_lock);

    if (new_device)
        new_device->ops.device.release(new_device);

    return ret;
}

int oe_umount2(const char* target, int flags)
{
    int ret = -1;
    size_t index = (size_t)-1;
    char suffix[OE_PATH_MAX];
    oe_device_t* device;
    bool locked = false;
    oe_syscall_path_t target_path;

    if (!target)
        OE_RAISE_ERRNO(EINVAL);

    /* Normalize the target path. */
    {
        if (!oe_realpath(target, &target_path))
            OE_RAISE_ERRNO(EINVAL);

        target = target_path.buf;
    }

    /* Resolve the device. */
    if (!(device = oe_mount_resolve(target, suffix)))
        OE_RAISE_ERRNO(EINVAL);

    pthread_spin_lock(&_lock);
    locked = true;

    /* Find and remove this device. */
    for (size_t i = 0; i < _mount_table_size; i++)
    {
        if (strcmp(_mount_table[i].path, target) == 0)
        {
            index = i;
            break;
        }
    }

    /* If mount point not found. */
    if (index == (size_t)-1)
        OE_RAISE_ERRNO(EINVAL);

    /* Remove the entry by swapping with the last entry. */
    {
        oe_device_t* fs = _mount_table[index].fs;

        free(_mount_table[index].path);
        _mount_table[index] = _mount_table[_mount_table_size - 1];
        _mount_table_size--;

        if (fs->ops.fs.umount2(fs, target, flags) != 0)
            OE_RAISE_ERRNO(errno);

        fs->ops.device.release(fs);
    }

    ret = 0;

done:

    if (locked)
        pthread_spin_unlock(&_lock);

    return ret;
}

int oe_umount(const char* target)
{
    return oe_umount2(target, 0);
}
