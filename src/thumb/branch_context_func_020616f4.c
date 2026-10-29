extern int func_02027698();
extern int func_02058b78();

unsigned int func_020616f4(unsigned int param_1,unsigned char *param_2)

{
    unsigned char *puVar1;
    int iVar2;

    if (param_2 == (unsigned char *)0x0) {
        return 0;
    }
    puVar1 = (unsigned char *)func_02027698(param_2);
    if ((puVar1 != (unsigned char *)0x0) && (iVar2 = func_02058b78(puVar1), iVar2 != 0)) {
        return 1;
    }
    return 0;
}
