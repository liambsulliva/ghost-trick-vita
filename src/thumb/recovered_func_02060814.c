extern int func_0205fe04();

unsigned int * func_02060814(unsigned int *param_1,unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = func_0205fe04(*param_1);
    *param_1 = uVar1;
    uVar1 = func_0205fe04(param_1[1],param_2);
    param_1[1] = uVar1;
    uVar1 = func_0205fe04(param_1[2],param_2);
    param_1[2] = uVar1;
    return param_1;
}
