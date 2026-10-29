extern int func_02029640();
extern int func_02051a70();

unsigned int func_02051a58(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02029640(*(unsigned int *)(param_1 + 0x38));
    func_02051a70(param_1,uVar1);
    return 1;
}
