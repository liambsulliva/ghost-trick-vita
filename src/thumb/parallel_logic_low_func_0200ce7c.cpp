extern "C" unsigned int func_0200cd20(void *, int);
extern "C" void *func_02009c1c();
extern "C" void func_0202f1bc(void *, void *, int, unsigned int, int);
struct Object {
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int);
    char pad[0x3c];
    int field;
    int pad2;
    short data[1];
};
extern "C" void func_0200ce7c(Object *object, int value)
{
    int offset = object->slot2(value) << 5;
    unsigned int result = func_0200cd20(object, value);
    void *target = func_02009c1c();
    func_0202f1bc(target, (char *)object->data + (offset & ~1), offset, result, object->field);
}
