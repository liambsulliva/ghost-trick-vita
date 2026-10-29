extern int WaitByLoop();
extern int func_0207389c();

void func_020737bc(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;

    iVar1 = func_0207389c();
    if (iVar1 <= 0) {
        return;
    }
    do {
        WaitByLoop(0x400);
        iVar1 = func_0207389c(param_1,param_2,param_3,param_4);
    } while (0 < iVar1);
    return;
}
