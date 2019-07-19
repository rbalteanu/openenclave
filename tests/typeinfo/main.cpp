// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/host.h>
#include <openenclave/internal/tests.h>
#include <openenclave/internal/typeinfo.h>
#include <stdlib.h>
#include <string.h>
#include <cwchar>
#include "CXXObject.h"
#include "typeinfo_u.h"

#define err(...)                      \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    } while (0)

static void __Test(const char* file, unsigned int line, const char* cond)
{
    fprintf(stderr, "%s(%u): %s: (test failed)\n", file, line, cond);
}

#define TEST(COND)                             \
    do                                         \
    {                                          \
        if (!(COND))                           \
        {                                      \
            __Test(__FILE__, __LINE__, #COND); \
            exit(1);                           \
        }                                      \
    } while (0)

OE_EXTERNC int32_t
UCopy(char* p, size_t m, const char* q, size_t n, struct Object** object)
{
    OE_UNUSED(p);
    OE_UNUSED(m);
    OE_UNUSED(q);
    OE_UNUSED(n);
    OE_UNUSED(object);
    return 0;
}

int InitObject(struct Object& object, size_t id, const char* name)
{
    memset(&object, 0, sizeof(Object));
    object.id = id;

    if (!(object.name = strdup(name)))
        return -1;

    return 0;
}

void DestroyObject(struct Object& object)
{
    OE_UNUSED(object);
}

struct Object* MakeObject(size_t id, const char* name)
{
    struct Object* p;

    if (!(p = (struct Object*)malloc(sizeof(struct Object))))
        return NULL;

    p->id = id;

    if (!(p->name = strdup(name)))
    {
        free(p);
        return NULL;
    }

    return p;
}

int TestContainer(bool trace)
{
    Object o;
    memset(&o, 0, sizeof(o));
    o.id = sizeof(o);
    o.name = (char*)"Object";

    Container c;
    memset(&c, 0, sizeof(c));

    uint32_t arr[] = {100, 200, 300};
    c.arrData = arr;
    c.arrSize = OE_COUNTOF(arr);

    if (trace)
        oe_print_struct(&Container_ti, &c);

    c.object = &o;
    c.size = 1;
    c.b = true;

    c.e.xxx = 1234;
    c.e.yyy = 5678;
    strcpy(c.e.str, "My char array");
    strcpy(c.e.cstr, "ABCDEFG\r\n\177");

    Object objects[2];
    objects[0].id = sizeof(Object);
    objects[0].name = (char*)("Object1");
    objects[1].id = sizeof(Object);
    objects[1].name = (char*)("Object1");
    c.e.objects = objects;
    c.e.nobjects = 2;

    memcpy(&c.ae[1], &c.e, sizeof(c.e));

    unsigned char varr[5] = {0, 1, 2, 3, 4};
    c.varr = varr;
    c.sizevarr = 5;

    if (trace)
        oe_print_struct(&Container_ti, &c);

    Container* s;
    oe_result_t result = oe_clone_struct(&Container_ti, &c, (void**)&s, malloc);

    if (result != OE_OK)
    {
        fprintf(stderr, "error: oe_clone_struct(): %u\n", result);
        exit(1);
    }

    if (trace)
        oe_print_struct(&Container_ti, s);

#if 0
    if ((result = oe_import_struct(&Container_ti, s)) != OE_OK)
    {
        fprintf(stderr, "error: oe_import_struct(): %u\n", result);
        exit(1);
    }
#endif

    oe_free_struct(&Container_ti, s, free);

    return 0;
}

static int TestObject(bool trace)
{
    oe_result_t result;
    Object* object = MakeObject(sizeof(Object), "Object");

    if (!object)
        err("MakeObject");

    TEST(!oe_pad_struct(&Object_ti, object));
    TEST(!oe_test_struct_padding(&Object_ti, object));

    if (trace)
        oe_print_struct(&Object_ti, object);

    if ((result = oe_free_struct(&Object_ti, object, free)))
        err("oe_free_struct");

    printf("=== passed TestObject()\n");
    return 0;
}

int TestCopyOver(bool trace)
{
    Object* obj1;
    Object* obj2;

    OE_UNUSED(trace);

    if (!(obj1 = MakeObject(10, "O10")))
        return -1;

    if (!(obj2 = MakeObject(20, "O20")))
        return -1;

    if (oe_free_struct(&Object_ti, obj1, free) != OE_OK)
        return -1;

    if (oe_free_struct(&Object_ti, obj2, free) != OE_OK)
        return -1;

    printf("=== passed TestCopyOver()\n");
    return 0;
}

int TestAllTypes(bool trace)
{
    const oe_struct_ti_t* sti = &AllTypes_ti;
    AllTypes x;
    AllTypes y;
    memset(&x, 0, sizeof(x));
    memset(&y, 0, sizeof(y));

    x.s8 = -8;
    x.u8 = 8;
    x.s16 = -16;
    x.u16 = 16;
    x.s32 = -32;
    x.u32 = 32;
    x.s64 = -64;
    x.u64 = 64;
    x.r32 = 32.0;
    x.r64 = 64.0;
    x.by = 0xBB;
    x.b = true;
    x.c = 'C';
    x.s = 99;
    x.ss = -99;
    x.str = strdup("str");
    x.strn = 4;
    strcpy(x.stra, "stra");
    x.obj1.id = 1;
    x.obj1.name = strdup("Object1");
    x.obj2 = MakeObject(2, "Object2");
    uint32_t* data = (uint32_t*)malloc(sizeof(uint32_t) * 3);
    data[0] = 0;
    data[1] = 1;
    data[2] = 2;
    x.data = data;
    x.size = 3;

    if (trace)
        oe_print_struct(&AllTypes_ti, &x);

    if (oe_copy_struct(&AllTypes_ti, &x, &y, malloc) != OE_OK)
        err("oe_copy_struct()");

    bool flag;
    if (oe_struct_eq(&AllTypes_ti, &x, &y, &flag) != OE_OK || !flag)
        err("oe_struct_eq()");

    if (trace)
        oe_print_struct(&AllTypes_ti, &y);

#if 0
    oe_print_struct(&AllTypes_ti, &x);
    oe_print_struct(&AllTypes_ti, &x);
#endif

    // Test oe_clear_arg():
    {
        AllTypes z;

        if (oe_copy_struct(sti, &y, &z, malloc) != OE_OK)
            err("oe_copy_struct()");

        TEST(!oe_clear_arg_by_name(sti, &y, "stra", 0, z.stra, free));
        TEST(strcmp(z.stra, "") == 0);
        TEST(!oe_clear_arg_by_name(sti, &y, "str", 0, z.str, free));
        TEST(strcmp(z.str, "") == 0);

        TEST(!oe_clear_arg_by_name(sti, &y, "str", 1, &z.str, free));
        TEST(z.str == NULL);
        TEST(!oe_clear_arg_by_name(sti, &y, "s8", 0, &z.s8, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "u8", 0, &z.u8, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "s16", 0, &z.s16, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "u16", 0, &z.u16, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "s32", 0, &z.s32, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "u32", 0, &z.u32, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "s64", 0, &z.s64, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "u64", 0, &z.u64, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "r32", 0, &z.r32, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "r64", 0, &z.r64, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "by", 0, &z.by, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "b", 0, &z.b, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "c", 0, &z.c, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "s", 0, &z.s, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "ss", 0, &z.ss, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "strn", 0, &z.strn, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "obj1", 0, &z.obj1, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "obj2", 1, &z.obj2, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "data", 1, &z.data, free));
        TEST(!oe_clear_arg_by_name(sti, &y, "size", 0, &z.size, free));

        AllTypes n;
        memset(&n, 0, sizeof(n));

        bool flag;
        TEST(!oe_struct_eq(&AllTypes_ti, &z, &n, &flag));
        TEST(flag);
    }

    // Test oe_set_arg():
    {
        AllTypes z;
        memset(&z, 0, sizeof(z));

        TEST(!oe_set_arg_by_name(sti, &y, "s8", 0, &z.s8, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "u8", 0, &z.u8, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "s16", 0, &z.s16, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "u16", 0, &z.u16, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "s32", 0, &z.s32, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "u32", 0, &z.u32, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "s64", 0, &z.s64, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "u64", 0, &z.u64, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "r32", 0, &z.r32, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "r64", 0, &z.r64, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "by", 0, &z.by, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "b", 0, &z.b, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "c", 0, &z.c, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "s", 0, &z.s, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "ss", 0, &z.ss, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "str", 1, &z.str, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "strn", 0, &z.strn, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "stra", 0, z.stra, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "obj1", 0, &z.obj1, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "obj2", 1, &z.obj2, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "data", 1, &z.data, malloc));
        TEST(!oe_set_arg_by_name(sti, &y, "size", 0, &z.size, malloc));

        bool flag;
        TEST(!oe_struct_eq(&AllTypes_ti, &z, &y, &flag));
        TEST(flag);

        oe_destroy_struct(&AllTypes_ti, &z, free);
    }

    oe_destroy_struct(&AllTypes_ti, &x, free);
    oe_destroy_struct(&AllTypes_ti, &y, free);

    printf("=== passed TestAllTypes()\n");
    return 0;
}

int MyFunctionCall(MyFunctionArgs* a)
{
    TEST(a->str);
    TEST(a->nstr);

    strncpy(a->str, "aaaaaaaaaaaaaaaaaaaaaa", a->nstr - 1);

    if (a->u32)
        *a->u32 = 66;

    a->u32a[0] = 10;
    a->u32a[1] = 20;
    a->u32a[2] = 30;
    a->u32a[3] = 40;

    *a->stra = '\0';
    strncat(a->stra, "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz", 31);

    a->objp->id = 11111111;
    a->objr->id = 22222222;

    return 0;
}

int MyFunction(
    const char* cstr,
    char* str,
    uint32_t nstr,
    uint32_t* u32,
    uint32_t u32a[4],
    char stra[3],
    struct Object obj,
    struct Object* objp,
    struct Object** objr)
{
    const oe_struct_ti_t* ti = &MyFunctionArgs_ti;
    struct MyFunctionArgs args;
    memset(&args, 0, sizeof(args));
    args.cstr = cstr;
    args.str = str;
    args.nstr = nstr;
    args.u32 = u32;
    memcpy(args.u32a, u32a, sizeof(args.u32a));
    memcpy(args.stra, stra, sizeof(args.stra));
    args.obj = obj;
    args.objp = objp;
    if (objr)
        args.objr = *objr;
    struct MyFunctionArgs* a;

    if (oe_clone_struct(ti, &args, (void**)&a, malloc) != OE_OK)
        return -1;

    bool flag = false;
    if (oe_struct_eq(ti, &args, a, &flag) != OE_OK || !flag)
        err("oe_struct_eq");

    if (MyFunctionCall(a) != 0)
        err("MyFunctionCall()");

#if 0
    oe_print_struct(&MyFunctionArgs_ti, a);
#endif

    TEST(!oe_set_arg_by_name(ti, a, "str", 0, str, malloc));
    TEST(!oe_set_arg_by_name(ti, a, "u32", 0, u32, malloc));
    TEST(!oe_set_arg_by_name(ti, a, "u32a", 0, u32a, malloc));
    TEST(!oe_set_arg_by_name(ti, a, "stra", 0, stra, malloc));
    TEST(!oe_clear_arg_by_name(ti, a, "objp", 0, objp, free));
    TEST(!oe_set_arg_by_name(ti, a, "objp", 0, objp, malloc));
    TEST(!oe_clear_arg_by_name(ti, a, "objr", 1, objr, free));
    TEST(!oe_set_arg_by_name(ti, a, "objr", 1, objr, malloc));

    oe_free_struct(ti, a, free);

    return 0;
}

static int TestParams(bool trace)
{
    uint32_t u32 = 99;

    OE_UNUSED(trace);

    char str[256];
    unsigned int nstr = sizeof(str);
    strcpy(str, "This is my 'str'");

    uint32_t u32a[4] = {1, 2, 3, 4};

    char stra[32];
    strcpy(stra, "stra");

    CXXObject obj(1, "obj");

    CXXObject* objp = new CXXObject(2, "objp");
    TEST(objp);

    CXXObject* objr = new CXXObject(3, "objr");
    TEST(objr);

    if (MyFunction(
            "cstr", str, nstr, &u32, u32a, stra, obj, objp, (Object**)&objr) !=
        0)
    {
        err("MyFunction() failed");
    }

    TEST(strcmp(str, "aaaaaaaaaaaaaaaaaaaaaa") == 0);
    TEST(u32 == 66);
    TEST(u32a[0] == 10);
    TEST(strcmp(stra, "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz") == 0);
    TEST(objp->id = 11111111);
    TEST(objr->id = 22222222);

    delete objp;
    delete objr;

    printf("=== passed TestParams()\n");
    return 0;
}

int main(int argc, const char* argv[])
{
    bool trace = false;

    OE_UNUSED(argc);
    OE_UNUSED(argv);

    if (TestObject(trace) != 0)
        return 1;

    if (TestContainer(trace) != 0)
        return 1;

    if (TestCopyOver(trace) != 0)
        return 1;

#if 1
    if (TestAllTypes(trace) != 0)
        return 1;
#endif

    if (TestParams(trace) != 0)
        return 1;

    printf("=== passed all tests (typeinfo)\n");

    return 0;
}