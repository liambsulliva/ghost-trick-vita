extern int func_02031288();

unsigned int * func_02022514(unsigned int *param_1,int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;

    *param_1 = param_3;
    param_1[1] = param_4;
    param_1[2] = param_2;
    uVar1 = func_02031288(param_2 << 4);
    param_1[3] = uVar1;
    return param_1;
}
