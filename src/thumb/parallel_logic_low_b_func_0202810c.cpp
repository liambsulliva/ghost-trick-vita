struct Inner {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3(void *buffer);
};
extern "C" Inner *func_02028148(void *object);
extern "C" void func_02020c98(unsigned int *object);
extern "C" int func_02020db0(int object,int value);
extern "C" void func_02020258(int object,int value);
extern "C" unsigned int *func_02020ca8(unsigned int *object);

extern "C" void func_0202810c(void *object,int value)
{
    unsigned int buffer[2];
    Inner *inner;
    int found;

    inner = func_02028148(object);
    if (inner != 0) {
        func_02020c98(buffer);
        inner->slot3(buffer);
        found = func_02020db0((int)buffer,0x02085110);
        if (found != 0) {
            func_02020258(found,value);
        }
        func_02020ca8(buffer);
    }
}
