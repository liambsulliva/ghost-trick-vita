class Object0203fdf8_0 {
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

void func_0203fdf8(int *param_1)

{
    func_0200b518(param_1 + 10,param_1[0x31],0x80000000,0x80000000);
    func_0200b598(param_1 + 10,param_1[0x2f],0xf);
    func_020280a8(param_1 + 10,param_1[0x30] & 0xff);
    func_0203fdd8(param_1 + 10,param_1[0x32] & 0xff);
    ((Object0203fdf8_0 *)param_1)->slot10();
    return;
}
}
