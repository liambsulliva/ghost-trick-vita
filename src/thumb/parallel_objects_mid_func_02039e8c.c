int *func_02039e8c(int *param_1)
{
    int *piVar1;
    int *piVar2;

    piVar1 = (int *)*param_1;
    if (piVar1 != (int *)0x0) {
        while ((piVar2 = (int *)*piVar1) != (int *)0x0) {
            piVar1 = piVar2;
        }
    }
    return piVar1;
}
