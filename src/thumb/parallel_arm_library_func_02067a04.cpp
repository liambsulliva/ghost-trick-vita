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

typedef int code(...);
extern "C" int func_0200b518(...);
extern "C" int func_0200b598(...);
extern "C" int func_020280a8(...);
extern "C" int func_0203fdd8(...);

extern "C" void func_02067a04(int *param_1)

{
    ((Object *)param_1)->slot10();
    func_0200b518(param_1 + 10,param_1[0x30],0x80000000,0x80000000);
    func_0200b598(param_1 + 10,*(unsigned char *)(param_1 + 0x2f) + 0x69,0xf);
    func_020280a8(param_1 + 10,0);
    func_0203fdd8(param_1 + 10,2);
    *(unsigned char *)((int)param_1 + 0xbd) = *(unsigned char *)((int)param_1 + 0xbc);
    return;
}
