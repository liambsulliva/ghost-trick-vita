extern int func_02038ae8();

unsigned int * func_0203b238(unsigned int *param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;

    func_02038ae8();
    uVar1 = ((unsigned int)0x020a6804u);
    param_1[4] = param_2;
    *param_1 = uVar1;
    *(unsigned short *)((int)param_1 + 10) = 0;
    *(unsigned short *)(param_1 + 3) = 0;
    param_1[5] = param_3;
    return param_1;
}
