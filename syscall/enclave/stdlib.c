// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/internal/syscall/bits/exports.h>
#include <openenclave/internal/syscall/limits.h>
#include <openenclave/internal/syscall/raise.h>
#include <openenclave/internal/syscall/stdio.h>
#include <openenclave/internal/syscall/stdlib.h>
#include <openenclave/internal/syscall/string.h>
#include <openenclave/internal/syscall/unistd.h>
#include <stdlib.h>
#include <string.h>

char* oe_realpath(const char* path, oe_syscall_path_t* resolved_path)
{
    char* ret = NULL;
    typedef struct _variables
    {
        char buf[OE_PATH_MAX];
        const char* in[OE_PATH_MAX];
        const char* out[OE_PATH_MAX];
        char resolved[OE_PATH_MAX];
    } variables_t;
    variables_t* v = NULL;
    size_t nin = 0;
    size_t nout = 0;

    if (!path)
        OE_RAISE_ERRNO(EINVAL);

    /* Allocate variables on the heap since too big for the stack. */
    if (!(v = calloc(1, sizeof(variables_t))))
        OE_RAISE_ERRNO(ENOMEM);

    if (path[0] == '/')
    {
        if (strlcpy(v->buf, path, sizeof(v->buf)) >= sizeof(v->buf))
            OE_RAISE_ERRNO(ENAMETOOLONG);
    }
    else
    {
        char cwd[OE_PATH_MAX];

        if (!oe_getcwd(cwd, sizeof(cwd)))
            OE_RAISE_ERRNO(EINVAL);

        if (strlcpy(v->buf, cwd, sizeof(v->buf)) >= sizeof(v->buf))
            OE_RAISE_ERRNO(ENAMETOOLONG);

        if (strlcat(v->buf, "/", sizeof(v->buf)) >= sizeof(v->buf))
            OE_RAISE_ERRNO(ENAMETOOLONG);

        if (strlcat(v->buf, path, sizeof(v->buf)) >= sizeof(v->buf))
            OE_RAISE_ERRNO(ENAMETOOLONG);
    }

    /* Split the path into elements. */
    {
        char* p;
        char* save;

        v->in[nin++] = "/";

        for (p = strtok_r(v->buf, "/", &save); p;
             p = strtok_r(NULL, "/", &save))
        {
            v->in[nin++] = p;
        }
    }

    /* Normalize the path. */
    for (size_t i = 0; i < nin; i++)
    {
        /* Skip "." elements. */
        if (strcmp(v->in[i], ".") == 0)
            continue;

        /* If "..", remove previous element. */
        if (strcmp(v->in[i], "..") == 0)
        {
            if (nout > 1)
                nout--;
            continue;
        }

        v->out[nout++] = v->in[i];
    }

    /* Build the resolved path. */
    {
        *v->resolved = '\0';

        for (size_t i = 0; i < nout; i++)
        {
            if (strlcat(v->resolved, v->out[i], OE_PATH_MAX) >= OE_PATH_MAX)
                OE_RAISE_ERRNO(ENAMETOOLONG);

            if (i != 0 && i + 1 != nout)
            {
                if (strlcat(v->resolved, "/", OE_PATH_MAX) >= OE_PATH_MAX)
                    OE_RAISE_ERRNO(ENAMETOOLONG);
            }
        }
    }

    if (resolved_path)
    {
        if (strlcpy(resolved_path->buf, v->resolved, OE_PATH_MAX) >=
            OE_PATH_MAX)
            OE_RAISE_ERRNO(ENAMETOOLONG);

        ret = resolved_path->buf;
        goto done;
    }
    else
    {
        char* p = strdup(v->resolved);

        if (!p)
            OE_RAISE_ERRNO(ENOMEM);

        ret = p;
        goto done;
    }

done:

    if (v)
        free(v);

    return ret;
}

OE_NO_RETURN void oe_exit(int status)
{
    OE_UNUSED(status);

    oe_host_printf("oe_exit() panic");
    abort();

    /* Never return. */
    for (;;)
        ;
}
