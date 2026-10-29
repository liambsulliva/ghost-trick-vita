typedef int code();
extern int func_02009900();
extern int func_020153d8();
extern void func_0202f7e8();
extern int func_0205693c();

void func_0204a1b8(unsigned int param_1,int *param_2,int param_3)

{
    int iVar1;
    unsigned int uVar2;
    code *pcVar3;

    if (*param_2 == 0) {
        iVar1 = func_0205693c(((unsigned int)0x020cf724u));
        *param_2 = iVar1;
        func_020153d8(iVar1,param_1);
        if (param_3 != 0) {
            pcVar3 = *(code **)(*(int *)*param_2 + 0x28);
        }
        else {
            pcVar3 = *(code **)(*(int *)*param_2 + 0x2c);
        }
        (*pcVar3)((int *)*param_2);
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,*param_2,1,0);
    }
    return;
}
