extern void func_020725c4(int *param_1, int param_2);

void func_02071ef0(int *param_1)
{
    unsigned int a;
    unsigned int b;

    a = param_1[11];
    b = param_1[19];
    param_1[11] = 0;
    param_1[19] = 0;
    func_020725c4(param_1, 0);
    param_1[11] = a;
    param_1[19] = b;
}
