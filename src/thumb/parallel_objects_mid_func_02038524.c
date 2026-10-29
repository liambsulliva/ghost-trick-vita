extern int func_02037194();

unsigned int func_02038524(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02037194();
    if ((int)uVar1 >= 0) {
        *(unsigned int *)(param_1 + 0x20) = uVar1;
        return 1;
    }
    return 0;
}
