struct Obj {
    char pad0[0x28];
    int w28;
    char pad2c[4];
    signed char b30;
    signed char b31;
    char pad32[2];
    unsigned char b34;
};

void func_02038618(struct Obj *p)
{
    p->b34 = 1;
    p->w28 = 1;
    p->b31 = p->b30 = 0;
}
