void func_ov004_020d6ea8(int param_1, unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = *(unsigned char *)(param_1 + 0xaa9);
    uVar1 = uVar1 & ~4u;
    *(unsigned char *)(param_1 + 0xaa9) = uVar1 | ((param_2 << 31) >> 29);
}
