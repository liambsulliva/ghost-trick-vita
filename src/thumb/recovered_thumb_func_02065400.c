extern int func_02065430();

void func_02065400(int param_1)

{
    unsigned int uVar1;

    uVar1 = 0;
    do {
        func_02065430(param_1,*(unsigned int *)(param_1 + uVar1 * 4 + 0x88),0);
        func_02065430(param_1,*(unsigned int *)(param_1 + uVar1 * 4 + 0x90),1);
        uVar1 = uVar1 + 1;
    } while (uVar1 < 2);
    return;
}
