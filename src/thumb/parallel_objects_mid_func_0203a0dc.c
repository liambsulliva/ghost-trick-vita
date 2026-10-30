extern unsigned int func_0203a0c0(unsigned int param_1, unsigned char *param_2, int param_3);

int func_0203a0dc(int *param_1, int param_2)
{
    int *piVar2;

    piVar2 = (int *)*param_1;
    while (piVar2 != (int *)0x0) {
        if (func_0203a0c0((unsigned int)param_1, (unsigned char *)piVar2[2], param_2) != 0) {
            return 1;
        }
        piVar2 = (int *)*piVar2;
    }
    return 0;
}
