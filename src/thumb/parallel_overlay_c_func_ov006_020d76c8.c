void func_ov006_020d76c8(unsigned char *param_1, unsigned int param_2)
{
    int iVar1;
    unsigned int uVar2;
    iVar1 = param_1[0xc2];
    uVar2 = param_2 << 31;
    iVar1 = iVar1 & ~4;
    param_1[0xc2] = iVar1 | (uVar2 >> 29);
}
