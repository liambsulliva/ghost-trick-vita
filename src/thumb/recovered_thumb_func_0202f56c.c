extern int func_02007aa8();
extern int func_02009900();
extern int func_0202f55c();

unsigned int * func_0202f56c(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02007aa8();
    uVar1 = ((unsigned int)0x020a5440u);
    param_1[1] = 0;
    *param_1 = uVar1;
    param_1[2] = 0;
    *(unsigned char *)(param_1 + 3) = 1;
    *(unsigned char *)((int)param_1 + 0xe) = 0;
    *(unsigned char *)((int)param_1 + 0xf) = 0;
    param_1[4] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    param_1[8] = 0;
    param_1[9] = 0;
    func_02009900();
    uVar1 = func_0202f55c();
    param_1[5] = uVar1;
    return param_1;
}
