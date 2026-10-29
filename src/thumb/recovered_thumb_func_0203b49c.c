extern int func_02022d70();

unsigned int * func_0203b49c(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02022d70();
    uVar1 = ((unsigned int)0x020a70a4u);
    param_1[8] = 0;
    *param_1 = uVar1;
    *(unsigned char *)(param_1 + 0xb) = 0;
    *(unsigned char *)((int)param_1 + 0x2d) = 0;
    *(unsigned char *)((int)param_1 + 0x2e) = 0;
    param_1[9] = 0;
    param_1[0xd] = 0;
    return param_1;
}
