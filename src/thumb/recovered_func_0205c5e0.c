extern int func_02056978();

unsigned int * func_0205c5e0(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02056978();
    uVar1 = ((unsigned int)0x020af158u);
    param_1[0x12] = 0;
    *param_1 = uVar1;
    *(unsigned char *)(param_1 + 0x14) = 0;
    param_1[0x13] = 0;
    param_1[0x15] = 0;
    return param_1;
}
