extern unsigned int func_0200e410();

unsigned char func_0201cf7c(int param_1,int param_2)
{
    unsigned int uVar1;
    unsigned char auStack[4];
    unsigned char bVar2;

    if ((*(char *)(param_1 + 0x36) != 0) && (*(int *)(param_1 + 0x38) != 0)) {
        uVar1 = func_0200e410(*(int *)(param_1 + 0x38),param_2);
        *(unsigned char *)((int)auStack + 0) = (unsigned char)uVar1;
        *(unsigned char *)((int)auStack + 1) = (unsigned char)(uVar1 >> 8);
        *(unsigned char *)((int)auStack + 2) = *(unsigned char *)((int)auStack + 0);
        *(unsigned char *)((int)auStack + 3) = *(unsigned char *)((int)auStack + 1);
        bVar2 = *(unsigned char *)((int)auStack + 1);
        if ((bVar2 != 0) || (*(unsigned char *)((int)auStack + 2) != 0)) {
            if ((bVar2 > 0x10) || (param_2 == 0xff)) {
                return (unsigned char)(*(char *)(param_1 + 0x3c) + *(char *)(param_1 + 0x3d));
            }
            return bVar2;
        }
    }
    return *(unsigned char *)(param_1 + 0x22);
}
