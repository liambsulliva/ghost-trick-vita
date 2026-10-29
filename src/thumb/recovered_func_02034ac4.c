extern int func_02034a0c();
extern int func_02034a14();
extern int func_0205902c();

void func_02034ac4(int param_1)

{
    int iVar1;

    iVar1 = func_02034a0c(*(unsigned int *)(param_1 + 0x24));
    if (iVar1 != 0) {
        func_0205902c(iVar1,0x88);
    }
    iVar1 = func_02034a14(*(unsigned int *)(param_1 + 0x24));
    if (iVar1 != 0) {
        func_0205902c(iVar1,0x88);
    }
    return;
}
