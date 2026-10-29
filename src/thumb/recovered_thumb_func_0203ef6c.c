extern int func_02022d70();
extern int func_02071e40();

unsigned int * func_0203ef6c(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02022d70();
    uVar1 = ((unsigned int)0x020a76b8u);
    param_1[8] = 0;
    *param_1 = uVar1;
    param_1[9] = 0;
    param_1[10] = 0;
    param_1[0xb] = 0;
    param_1[0xc] = 0;
    param_1[0xd] = 0;
    param_1[0xe] = 0;
    param_1[0xf] = 0;
    param_1[0x10] = 0;
    param_1[0x11] = 0;
    uVar1 = func_02071e40();
    param_1[0x12] = uVar1;
    *(unsigned char *)(param_1 + 0x14) = 0;
    *(unsigned char *)((int)param_1 + 5) = 1;
    param_1[0x13] = 0;
    return param_1;
}
