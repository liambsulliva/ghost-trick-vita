class Object020270b0_0 {
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
extern char data_020cdcf0;
extern char data_020cf87c;
typedef int code(...);
extern int func_02008828(...);
extern int func_0201123c(...);
extern int func_02011264(...);
extern int func_02022fc0(...);
extern int func_020266d4(...);
extern int func_02036fd0(...);

void func_020270b0(unsigned int param_1,int *param_2)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;

    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,0);
    iVar2 = func_02022fc0(uVar1,((unsigned int)&data_020cdcf0));
    if (((iVar2 != 0) && (iVar3 = func_020266d4(param_2), iVar3 != 0)) &&
          (iVar3 = func_02008828(iVar3,((unsigned int)&data_020cf87c)), iVar3 != 0)) {
        uVar1 = func_020266d4(param_2);
        func_02036fd0(iVar2,uVar1);
    }
    ((Object020270b0_0 *)param_2)->slot8((int)(0));
    return;
}
}
