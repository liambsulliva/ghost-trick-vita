extern int func_0202f524();

unsigned int * func_02010240(unsigned int *param_1)

{
    unsigned int uVar1;

    func_0202f524();
    uVar1 = ((unsigned int)0x020a8adcu);
    param_1[10] = 0;
    *param_1 = uVar1;
    param_1[0xb] = 0xffffffff;
    *(unsigned short *)(param_1 + 0xc) = 0;
    return param_1;
}
