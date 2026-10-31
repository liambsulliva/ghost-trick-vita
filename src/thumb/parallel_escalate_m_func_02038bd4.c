extern void func_02039dbc();
extern void func_02039d58();

void func_02038bd4(int *param_1, unsigned int param_2, int param_3)
{
    int iVar1;

    if (param_3 < 0) {
        iVar1 = param_1[7] + 3;
        if (iVar1 < param_2) {
            func_02039dbc(*param_1, iVar1, 5);
            return;
        }
    }
    if (0 < param_3) {
        iVar1 = param_1[7] - 3;
        if (iVar1 >= 0) {
            func_02039d58(*param_1, iVar1, -1);
        }
    }
}
