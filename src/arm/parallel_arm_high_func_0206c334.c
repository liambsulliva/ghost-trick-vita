int func_0206c334(unsigned char *param_1, int param_2)
{
    int iVar1;

    iVar1 = param_2 - 1;
    while (iVar1 > 0 && (unsigned int)((param_1[iVar1 + -1] ^ 0x20) - 0xa1) < 0x3c) {
        iVar1 = iVar1 - 1;
    }
    return (param_2 - 1) - (((param_2 - 1) - iVar1) & 1);
}
