class Object0205902c_0 {
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
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
};

extern "C" {
typedef int code(...);

void func_0205902c(int *param_1,int param_2)

{
    if (param_2 < 0) {
        ((Object0205902c_0 *)param_1)->slot15();
        return;
    }
    *(short *)(param_1 + 0x30) = (short)param_2;
    return;
}
}
