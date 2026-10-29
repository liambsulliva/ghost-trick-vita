extern char data_020b18a0;
extern int func_02000d9c();

unsigned int func_020063c8(unsigned int param_1,int param_2,unsigned char param_3)

{
    int iVar1;
    int iVar2;

    iVar2 = ((int)&data_020b18a0);
    iVar1 = *(int *)(*(int *)(((int)&data_020b18a0) + 0x10) + 0x1580);
    iVar1 = func_02000d9c(iVar1,param_1,*(unsigned int *)(*(int *)(iVar1 + 0x98) + 0x24));
    if (iVar1 == 0) {
        return 0xfffffffb;
    }
    iVar2 = *(int *)(iVar2 + 0x10) + ((int)0x00003d28u) + (unsigned int)*(unsigned char *)(iVar1 + 6) * 0x3a4;
    if ((*(unsigned int *)(iVar2 + 0xe8) & 1) == 0) {
        return 0xfffffffa;
    }
    if (*(unsigned char *)(iVar2 + param_2 + 0xf6) == 0xffffffff) {
        return 0xfffffff6;
    }
    *(unsigned char *)(iVar2 + param_2 + 0xfa) = param_3;
    *(unsigned char *)(iVar2 + 0xf1) = *(unsigned char *)(iVar2 + 0xf1) | 2;
    return 0;
}
