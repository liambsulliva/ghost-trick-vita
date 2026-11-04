struct Obj {
    unsigned char pad[0x142];
    unsigned char f142;
    unsigned char f143 : 8;
};

void func_02026744(void *obj, int a)
{
    struct Obj *o;
    o = (struct Obj *)obj;
    o->f142 |= 0x80;
    o->f143 = a;
}
