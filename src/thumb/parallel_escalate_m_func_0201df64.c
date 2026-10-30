unsigned int func_0201df64(int param_1,unsigned int param_2,unsigned short param_3,unsigned short param_4)
{
    int iVar1;
    int iVar2;
    int iVar3;

    if (2 < param_2) {
        return 0;
    }
    iVar2 = param_2 * 0x1c;
    iVar1 = param_1 + iVar2;
    if (*(unsigned char *)(iVar1 + 0x38) >= 4) {
        return 0;
    }
    iVar3 = param_1 + 0x38;
    *(char *)(iVar3 + iVar2) = *(char *)(iVar3 + iVar2) + '\x01';
    iVar2 = iVar1 + (unsigned int)*(unsigned char *)(iVar3 + iVar2) * 4;
    *(unsigned short *)(iVar2 + 0x3a) = param_3;
    *(unsigned short *)(iVar2 + 0x3c) = param_4;
    return 1;
}
