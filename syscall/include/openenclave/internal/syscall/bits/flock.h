// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

struct __OE_FLOCK
{
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};
