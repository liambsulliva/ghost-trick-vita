class Object02034aec_0 {
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
};

class Object02034aec_1 {
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
};

extern "C" {
typedef int code(...);
extern int func_02034a0c(...);
extern int func_02034a14(...);

void func_02034aec(int param_1)

{
    int *piVar1;

    piVar1 = (int *)func_02034a0c(*(unsigned int *)(param_1 + 0x24));
    if (piVar1 != (int *)0x0) {
        ((Object02034aec_0 *)piVar1)->slot13();
    }
    piVar1 = (int *)func_02034a14(*(unsigned int *)(param_1 + 0x24));
    if (piVar1 != (int *)0x0) {
        ((Object02034aec_1 *)piVar1)->slot14();
    }
    return;
}
}
