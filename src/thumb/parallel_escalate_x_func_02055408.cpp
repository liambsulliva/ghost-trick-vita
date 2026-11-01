struct Item55408 {
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    char pad[0xc4];
};

struct Obj55408 {
    char pad[0x28];
    Item55408 *items;
    char pad2[0x60];
    unsigned short last;
    unsigned short count;
};

extern "C" void func_02055408(Obj55408 *object)
{
    unsigned short i;
    if (object->items != 0) {
        for (i = 0; i < object->count; i++) {
            if (i <= object->last) {
                object->items[i].v01c();
            }
        }
    }
}
