extern int func_02022d70();

unsigned int * func_020368ac(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02022d70();
    uVar1 = ((unsigned int)0x020a6770u);
    param_1[10] = 0;
    *param_1 = uVar1;
    *(unsigned char *)(param_1 + 0xd) = 0;
    param_1[0xb] = 0;
    return param_1;
}
