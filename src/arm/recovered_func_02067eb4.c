void func_02067eb4(unsigned int *param_1,unsigned short param_2)

{
    *param_1 = 0;
    param_1[1] = 0;
    *(unsigned short *)(param_1 + 2) = 0;
    *(unsigned short *)((int)param_1 + 10) = param_2;
    return;
}
