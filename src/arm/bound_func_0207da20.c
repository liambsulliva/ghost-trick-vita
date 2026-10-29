extern char data_020d3a3c;
void func_0207da20(int param_1,int param_2,int *param_3)

{
    int *piVar1;

    piVar1 = ((int *)&data_020d3a3c);
    *param_3 = *((int *)&data_020d3a3c);
    param_3[1] = param_2;
    param_3[2] = param_1;
    *piVar1 = (int)param_3;
    return;
}
