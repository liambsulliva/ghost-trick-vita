extern int func_02038b34();

unsigned int func_0203a0c0(unsigned int param_1,unsigned char *param_2,int param_3)

{
    int iVar1;

    if ((param_2 != (unsigned char *)0x0) && (iVar1 = func_02038b34(param_2), param_3 == iVar1)) {
        return 1;
    }
    return 0;
}
