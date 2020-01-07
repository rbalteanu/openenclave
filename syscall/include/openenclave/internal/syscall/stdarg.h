// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#ifndef _OE_SYSCALL_STDARG_H
#define _OE_SYSCALL_STDARG_H

#ifndef oe_va_list
#define oe_va_list __builtin_va_list
#endif

#ifndef oe_va_start
#define oe_va_start __builtin_va_start
#endif

#ifndef oe_va_arg
#define oe_va_arg __builtin_va_arg
#endif

#ifndef oe_va_end
#define oe_va_end __builtin_va_end
#endif

#ifndef oe_va_copy
#define oe_va_copy __builtin_va_copy
#endif

#endif /* _OE_SYSCALL_STDARG_H */
