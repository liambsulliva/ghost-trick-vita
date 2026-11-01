extern "C" void func_0204f4d8(void *object);

struct Actor56048 {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38(int value);
    char pad[0xcc];
    int n;
    int a;
    int b;
};

extern "C" void func_02056048(Actor56048 *object)
{
    func_0204f4d8(object);
    if (object->n <= 0x20) {
        int lo = object->a << 12;
        int span = (object->b << 12) - lo;
        object->v38(lo + ((object->n * span) >> 5));
        object->n = object->n + 1;
    }
}
