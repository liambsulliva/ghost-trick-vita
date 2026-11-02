struct Obj {
    unsigned char pad[0x38];
    unsigned char a;
    unsigned char b;
};

unsigned int func_02042b30(struct Obj *p)
{
    if (p->a != 0xff) {
        return 0;
    }
    if (p->b == 0xff) {
        return 1;
    }
    return 0;
}
