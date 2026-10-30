extern int func_020274ac();

int func_02027424(unsigned int param_1)
{
    int iVar1;
    unsigned int uVar2;

    if (param_1 == 0x1000) {
        return 0;
    }
    iVar1 = func_020274ac(param_1);
    uVar2 = param_1 & 0xffff6fff;
    return ((int)(unsigned int)*(unsigned char *)(iVar1 + (uVar2 >> 3)) >> (uVar2 & 7) & 1) != 0;
}
