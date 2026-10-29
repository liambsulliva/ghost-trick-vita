class Object0204bc40_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int argument0);
};

class Object0204bc40_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int argument0);
};

extern "C" {
typedef int code(...);
extern int func_02008444(...);
extern int func_02031af4(...);
extern int func_02031b04(...);

void func_0204bc40(int *param_1)

{
    unsigned int uVar1;
    int iVar2;

    if (param_1[0x2e] == 0) {
        ((Object0204bc40_0 *)param_1)->slot8((int)(0));
    }
    uVar1 = func_02008444();
    iVar2 = func_02031af4(uVar1,param_1[0x2f]);
    if (iVar2 != 0) {
        param_1[0x2e] = 0;
        uVar1 = func_02008444();
        func_02031b04(uVar1,param_1 + 0x2f);
        ((Object0204bc40_1 *)param_1)->slot8((int)(0));
    }
    return;
}
}
