int func_0200c3fc(int param_1)

{
    int iVar1;

    iVar1 = (unsigned int)*(unsigned short *)(param_1 + 2) * 0x800000;
    return (int)(iVar1 + ((unsigned int)(iVar1 >> 0x16) >> 9)) >> 0x17;
}
