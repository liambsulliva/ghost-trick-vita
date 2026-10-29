extern int func_0200ba28();

unsigned int func_ov005_020d4420(int param_1)

{
    int iVar1;

    iVar1 = func_0200ba28(param_1 + 0x28);
    if (*(int *)(param_1 + 0xe4) == iVar1) {
        return 1;
    }
    return 0;
}
