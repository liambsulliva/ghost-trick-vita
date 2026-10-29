extern int func_02008444();
extern int func_02022dfc();
extern int func_02022f34();
extern int func_020322d4();

void func_02037d3c(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_02008444();
    iVar1 = func_020322d4();
    if ((iVar1 == 0) && (*(int *)(param_1 + 0x50) != 2)) {
        uVar2 = func_02022dfc(param_1);
        func_02022f34(uVar2,((unsigned int)0x0209a888u),0);
        *(unsigned int *)(param_1 + 0x50) = 2;
    }
    return;
}
