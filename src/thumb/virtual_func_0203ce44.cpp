class Object0203ce44_0 {
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

void func_0203ce44(int *param_1)

{
    int *piVar1;

    *(unsigned char *)(param_1 + 0x11) = 0;
    param_1[0xf] = 0x1f;
    piVar1 = ((int *)0x0209b690u);
    param_1[0x10] = 0;
    param_1[0xe] = *piVar1;
    ((Object0203ce44_0 *)param_1)->slot10();
    return;
}
}
