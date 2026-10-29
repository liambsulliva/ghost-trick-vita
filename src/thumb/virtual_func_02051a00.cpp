class Object02051a00_0 {
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
extern int func_02023fdc(...);
extern int func_02051448(...);

void func_02051a00(int *param_1)

{
    int iVar1;

    do {
        iVar1 = func_02023fdc(param_1 + 10);
    } while (iVar1 != 0);
    iVar1 = func_02051448(param_1 + 10);
    if (iVar1 != 0) {
        ((Object02051a00_0 *)param_1)->slot8((int)(0));
    }
    return;
}
}
