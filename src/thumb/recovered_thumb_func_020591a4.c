extern int func_02008444();
extern int func_020101f0();
extern int func_020266dc();
extern int func_020266f4();

void func_020591a4(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    uVar1 = func_020266f4();
    if (param_2 < uVar1) {
        uVar2 = func_020266dc(param_1,param_2);
        uVar3 = func_02008444();
        func_020101f0(uVar3,uVar2,param_3);
    }
    return;
}
