class Object020569b4_0 {
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
extern int func_0202ffb4(...);

int * func_020569b4(int *param_1)

{
    func_0202ffb4();
    *param_1 = ((int)0x020adb58u);
    *(unsigned short *)((int)param_1 + 0x42) = 0;
    *(unsigned char *)(param_1 + 0x11) = 0x1f;
    *(unsigned char *)((int)param_1 + 0x45) = 1;
    *(unsigned short *)((int)param_1 + 0x46) = 0x100;
    ((Object020569b4_0 *)param_1)->slot10();
    return param_1;
}
}
