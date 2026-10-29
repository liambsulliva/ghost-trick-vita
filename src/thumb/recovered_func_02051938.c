extern int func_02027c28();

void func_02051938(int param_1,unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = func_02027c28(param_2);
    *(unsigned int *)(param_1 + 0xfc) = uVar1;
    return;
}
