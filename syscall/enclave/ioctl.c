// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/internal/syscall/device.h>
#include <openenclave/internal/syscall/fdtable.h>
#include <openenclave/internal/syscall/raise.h>
#include <openenclave/internal/syscall/stdio.h>
#include <stdarg.h>

int __oe_ioctl(int fd, unsigned long request, uint64_t arg)
{
    int ret = -1;
    oe_fd_t* desc;

    if (!(desc = oe_fdtable_get(fd, OE_FD_TYPE_ANY)))
        OE_RAISE_ERRNO(errno);

    ret = desc->ops.fd.ioctl(desc, request, arg);

done:
    return ret;
}

int oe_ioctl(int fd, unsigned long request, ...)
{
    va_list ap;
    va_start(ap, request);
    int r = __oe_ioctl(fd, request, va_arg(ap, uint64_t));
    va_end(ap);
    return r;
}
