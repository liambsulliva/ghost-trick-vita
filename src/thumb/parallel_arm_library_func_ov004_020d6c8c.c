void func_ov004_020d6c8c(unsigned int *param_1, unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = param_1[2];
    uVar1 = uVar1 & ~8u;
    param_1[2] = uVar1 | ((param_2 << 31) >> 28);
}
