class Object02027f2c_0 {
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
    virtual void slot12(int argument0);
};

extern "C" {
extern char data_020cb04c;
extern char data_020cb068;
typedef int code(...);
extern int func_02008828(...);
extern int func_02014ddc(...);
extern int func_02014de4(...);
extern int func_02028148(...);

void func_02027f2c(unsigned int param_1,unsigned int *param_2)

{
    int *piVar1;
    int iVar2;

    piVar1 = (int *)func_02028148();
    if (piVar1 != (int *)0x0) {
        iVar2 = func_02008828(piVar1,((unsigned int)&data_020cb068));
        if (iVar2 != 0) {
            ((Object02027f2c_0 *)piVar1)->slot12((int)(param_2));
            return;
        }
        iVar2 = func_02008828(piVar1,((unsigned int)&data_020cb04c));
        if (iVar2 != 0) {
            func_02014ddc(piVar1,*param_2);
            func_02014de4(piVar1,param_2[1]);
        }
    }
    return;
}
}
