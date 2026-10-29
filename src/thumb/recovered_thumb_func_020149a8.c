extern int func_02014944();

unsigned int * func_020149a8(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02014944();
    uVar1 = ((unsigned int)0x020a3450u);
    param_1[0x15] = 0;
    *param_1 = uVar1;
    *(unsigned char *)(param_1 + 0x18) = 0;
    *(unsigned char *)((int)param_1 + 0x61) = 0;
    *(unsigned char *)((int)param_1 + 0x62) = 0;
    param_1[0x16] = 0;
    param_1[0x17] = 0;
    return param_1;
}
