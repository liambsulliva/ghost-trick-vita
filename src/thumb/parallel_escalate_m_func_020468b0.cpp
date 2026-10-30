class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
};

extern "C" {
extern char data_020a8a28;
extern char data_020cf724;
extern char data_020ce7a0;
extern void *func_0202f524(void *object);
extern void *func_0205693c(void *heap);
extern void *func_020465c4(void *heap);
extern void func_020153d8(void *kid, void *owner);
}

extern "C" char *func_020468b0(char *object)
{
    void **slot;
    unsigned int i;
    func_0202f524(object);
    *(void **)object = &data_020a8a28;
    for (i = 0; i < 4; i++) {
        slot = (void **)(object + i * 4);
        slot[11] = func_0205693c(&data_020cf724);
        func_020153d8(slot[11], object);
    }
    *(void **)(object + 0x28) = func_020465c4(&data_020ce7a0);
    func_020153d8(*(void **)(object + 0x28), object);
    ((Object *)object)->slot10();
    return object;
}
