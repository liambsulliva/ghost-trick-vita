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
    virtual void slot11();
};

extern "C" {
extern int data_020ad5e8;
extern int data_020cf724;
extern void func_0202f524(...);
extern void func_0200f70c(...);
extern int func_0205693c(...);
extern int func_02009900(...);
extern void func_0202f7e8(...);
extern void func_020153d8(...);
}

extern "C" int *func_02055b9c(int *param_1)
{
    unsigned int offset;
    int *kid;
    int *slot;
    unsigned int i;

    func_0202f524(param_1);
    *param_1 = (int)&data_020ad5e8;
    *(unsigned char *)(param_1 + 10) = *(unsigned char *)(param_1 + 10) & ~3;
    func_0200f70c(param_1 + 0xf);
    ((Object *)param_1)->slot11();
    i = 0;
    do {
        offset = i * 4;
        slot = (int *)((char *)param_1 + offset);
        slot[0x12] = func_0205693c(&data_020cf724);
        func_0202f7e8(func_02009900(),slot[0x12],7,0);
        kid = (int *)((int)param_1 + offset);
        func_020153d8(kid[0x12],param_1);
        ((Object *)kid[0x12])->slot11();
        i = i + 1;
    } while (i < 4);
    return param_1;
}
