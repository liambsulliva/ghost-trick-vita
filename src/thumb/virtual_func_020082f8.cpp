class Object020082f8_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual int slot4();
};

extern "C" {
typedef int code(...);
extern int func_02008314(...);

int * func_020082f8(int *param_1,unsigned int param_2)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = ((Object020082f8_0 *)param_1)->slot4();
    iVar2 = func_02008314(uVar1,param_2);
    if (iVar2 == 0) {
        param_1 = (int *)0x0;
    }
    return param_1;
}
}
