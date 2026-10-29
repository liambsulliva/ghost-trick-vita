extern int func_0200aca8();
extern int func_0203a134();

unsigned int * func_0203a104(unsigned int *param_1)

{
    func_0203a134();
    *param_1 = ((unsigned int)0x020a686cu);
    param_1[0x33] = 0;
    *(unsigned short *)(param_1 + 0x34) = 0;
    *(unsigned short *)((int)param_1 + 0xd2) = 0;
    func_0200aca8(param_1 + 0x35);
    return param_1;
}
