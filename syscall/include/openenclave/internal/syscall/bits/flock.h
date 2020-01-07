// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

struct __OE_FLOCK
{
    short l_type;
    short l_whence;
    oe_off_t l_start;
    oe_off_t l_len;
    oe_pid_t l_pid;
};
