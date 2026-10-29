class Object0204117c_0 {
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
    virtual int slot12();
};

extern "C" {
typedef int code(...);
extern int func_020410d8(...);
extern int func_020411e4(...);

void func_0204117c(unsigned short *param_1)

{
    int iVar1;
    unsigned int uVar2;

    if ((*param_1 == 1) && (iVar1 = func_020410d8(*(int *)(param_1 + 2)), iVar1 == 0)) {
        uVar2 = ((Object0204117c_0 *)(*(int **)(param_1 + 2)))->slot12();
        if ((unsigned short)param_1[1] != uVar2) {
            func_020411e4(param_1);
            return;
        }
        *param_1 = 2;
    }
    return;
}
}
