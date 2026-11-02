struct Flags {
    unsigned char b0 : 1;
};

struct Obj {
    unsigned char pad0[0xd5];
    unsigned char a;
    unsigned char pad1[1];
    unsigned char b;
    unsigned char pad2[1];
    struct Flags flags;
};

int func_02058414(struct Obj *p)
{
    if (p->a == p->b && p->flags.b0 == 0) {
        return 1;
    }
    return 0;
}
