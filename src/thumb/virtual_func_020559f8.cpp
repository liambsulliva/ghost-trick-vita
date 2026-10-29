class Object020559f8_0 {
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
typedef int code(...);
extern int func_0200b518(...);
extern int func_0200b598(...);
extern int func_020280a8(...);
extern int func_0203fdd8(...);

void func_020559f8(int *param_1)

{
    func_0200b518(param_1 + 10,0,0x80000000,0x80000000);
    func_0200b598(param_1 + 10,((unsigned int)0x00001b5fu),0xf);
    func_0203fdd8(param_1 + 10,2);
    func_020280a8(param_1 + 10,10);
    ((Object020559f8_0 *)param_1)->slot10();
    param_1[0x30] = 0x9000;
    *(unsigned char *)(param_1 + 0x31) = 1;
    return;
}
}
