class Object02033110_0 {
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

void func_02033110(int *param_1)

{
    ((Object02033110_0 *)param_1)->slot11();
    *(unsigned char *)((int)param_1 + 0x55) = 1;
    return;
}
}
