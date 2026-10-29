extern int func_0202f524();

unsigned int * func_02014944(unsigned int *param_1)

{
    unsigned int uVar1;

    func_0202f524();
    uVar1 = ((unsigned int)0x020a3494u);
    param_1[10] = 0;
    *param_1 = uVar1;
    param_1[0xb] = 0xffffffff;
    param_1[0xd] = 0xffffffff;
    *(unsigned char *)(param_1 + 0xe) = 0;
    *(unsigned char *)((int)param_1 + 0x39) = 0;
    *(unsigned short *)(param_1 + 0xc) = 0;
    *(unsigned short *)((int)param_1 + 0x32) = 0;
    param_1[0xf] = 0;
    param_1[0x10] = 0;
    param_1[0x11] = 0;
    param_1[0x12] = 0;
    param_1[0x13] = 0;
    param_1[0x14] = 0;
    return param_1;
}
