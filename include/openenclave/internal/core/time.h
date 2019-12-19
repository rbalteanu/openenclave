// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_INTERNAL_CORE_TIME_H
#define _OE_INTERNAL_CORE_TIME_H

#include <openenclave/internal/defs.h>
#include <openenclave/internal/types.h>

OE_EXTERNC_BEGIN

struct oe_tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

struct oe_timespec;

struct oe_tm* oe_gmtime_r(const time_t* timep, struct oe_tm* result);

int oe_sleep_msec(uint64_t milliseconds);

uint64_t oe_get_time(void);

time_t oe_time(time_t* tloc);

OE_EXTERNC_END

#endif /* _OE_INTERNAL_CORE_TIME_H */
