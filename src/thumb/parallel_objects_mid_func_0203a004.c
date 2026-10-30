extern int func_02038b34(signed char *object);

void func_0203a004(int *param_1)
{
    int *piVar2;

    piVar2 = (int *)*param_1;
    if (piVar2 == (int *)0x0) {
        return;
    }
    do {
        do {
        } while (piVar2[2] == 0);
        if (func_02038b34((signed char *)piVar2[2]) == 2) {
            param_1[1] = (int)piVar2;
            return;
        }
        piVar2 = (int *)*piVar2;
    } while (piVar2 != (int *)0x0);
}
