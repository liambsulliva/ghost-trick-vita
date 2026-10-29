extern int func_0200b598();
extern int func_0200ba28();

void func_02067da8(int param_1)

{
    int iVar1;
    int iVar2;

    iVar2 = *(int *)(param_1 + 0xe0);
    iVar1 = func_0200ba28(param_1 + 0x28);
    if (iVar2 != iVar1) {
        func_0200b598(param_1 + 0x28,iVar2,0xf);
    }
    *(unsigned int *)(param_1 + 0x10) = 0;
    return;
}
