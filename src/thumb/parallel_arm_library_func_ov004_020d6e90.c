void func_ov004_020d6e90(int param_1, unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = *(unsigned char *)(param_1 + 0xaa9);
    uVar1 = uVar1 & ~2u;
    *(unsigned char *)(param_1 + 0xaa9) = uVar1 | ((param_2 << 31) >> 30);
}
