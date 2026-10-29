extern int func_0202f64c();
extern int func_02031270();

int * func_02039d2c(int *param_1)

{
    int *piVar1;

    piVar1 = (int *)*param_1;
    while (piVar1 != (int *)0x0) {
        piVar1 = (int *)*piVar1;
        func_0202f64c(*param_1 + 8);
        if (*param_1 != 0) {
            func_02031270();
            *param_1 = 0;
        }
        *param_1 = (int)piVar1;
    }
    return param_1;
}
