class Object02046848_0 {
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

void func_02046848(int *param_1)

{
    func_0200b518(param_1 + 10,1,0x80000000,0x80000000);
    func_0200b598(param_1 + 10,0x6b,0xf);
    func_020280a8(param_1 + 10,1);
    func_0203fdd8(param_1 + 10,2);
    ((Object02046848_0 *)param_1)->slot10();
    return;
}
}
