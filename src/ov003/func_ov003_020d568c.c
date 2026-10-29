extern int func_02022dfc();
extern int func_02022f34();

void func_ov003_020d568c(int param_1)

{
    unsigned int uVar1;

    *(unsigned char *)(param_1 + 0x168) = 4;
    *(unsigned int *)(param_1 + 0x174) = 0;
    uVar1 = func_02022dfc();
    func_02022f34(uVar1,((unsigned int)0x020d5b20u),0);
    return;
}
