int func_02068adc(int *param_1, unsigned int *param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned int *puVar1;
    puVar1 = (unsigned int *)*param_2;
    if (puVar1 != 0) {
        *param_2 = puVar1[3];
    }
    if (puVar1 != 0) {
        *puVar1 = param_3;
        puVar1[1] = param_4;
        puVar1[2] = 0;
        puVar1[3] = 0;
        if (*param_1 != 0) {
            ((unsigned int *)*param_1)[2] = (unsigned int)puVar1;
        }
        puVar1[3] = *param_1;
        puVar1[2] = 0;
        *param_1 = (int)puVar1;
        return 1;
    }
    return 0;
}
