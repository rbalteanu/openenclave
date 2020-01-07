// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

struct __OE_DIRENT
{
    uint64_t d_ino;
    off_t d_off;
    uint16_t d_reclen;
    uint8_t d_type;
    char d_name[NAME_MAX + 1];
};
