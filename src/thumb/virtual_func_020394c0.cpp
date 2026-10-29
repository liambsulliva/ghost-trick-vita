class Object020394c0_0 {
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
typedef int code(...);
extern int func_020394e4(...);

void func_020394c0(int *param_1)

{
    *(unsigned char *)(param_1 + 4) = 0x32;
    *(unsigned char *)((int)param_1 + 0xc1) = 0x14;
    func_020394e4(param_1 + 10,3);
    ((Object020394c0_0 *)param_1)->slot11();
    return;
}
}
