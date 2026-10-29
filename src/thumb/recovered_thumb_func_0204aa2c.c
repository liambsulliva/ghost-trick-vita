extern int func_02049bf0();

unsigned int * func_0204aa2c(unsigned int *param_1)

{
    int iVar1;

    func_02049bf0();
    *param_1 = ((unsigned int)0x020a9e70u);
    *(unsigned char *)(param_1 + 0x2e) = 3;
    iVar1 = ((int)0x00000121u);
    *(unsigned char *)((int)param_1 + ((int)0x00000121u)) = 1;
    *(unsigned char *)((int)param_1 + iVar1 + 1) = 1;
    return param_1;
}
