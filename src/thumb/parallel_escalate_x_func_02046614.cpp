class Kid {
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
    virtual void slot11();
};
extern "C" {
extern char data_020a8a98;
extern char data_020cf724;
extern void *func_0202f524(void *object);
extern Kid *func_0205693c(void *heap);
extern void func_020153d8(Kid *kid, void *owner);
char *func_02046614(char *object)
{
    unsigned int i;
    Kid **slot;
    func_0202f524(object);
    *(void **)object = &data_020a8a98;
    for (i = 0; i < 2; i++) {
        slot = (Kid **)(object + i * 4);
        slot[10] = func_0205693c(&data_020cf724);
        slot[10]->slot11();
        func_020153d8(*(Kid **)((int)object + i * 4 + 0x28), object);
    }
    *(int *)(object + 0x38) = 0;
    *(int *)(object + 0x3c) = 0;
    *(int *)(object + 0x40) = 0x20;
    *(unsigned char *)(object + 0x44) = 2;
    return object;
}
}
