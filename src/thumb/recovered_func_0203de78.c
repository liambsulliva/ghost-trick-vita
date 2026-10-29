extern int func_02022dd8();
extern int func_02061138();

void func_0203de78(int param_1)

{
    int iVar1;

    iVar1 = func_02022dd8();
    *(int *)(param_1 + 0x20) = iVar1;
    func_02061138(*(unsigned int *)(iVar1 + 0x24),0);
    *(unsigned int *)(param_1 + 0x28) = 0x5a;
    return;
}
