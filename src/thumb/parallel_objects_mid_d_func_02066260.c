unsigned char func_02066260(int param_1, unsigned int param_2)
{
    int model;

    model = *(int *)(param_1 + 0xc8);
    if (model == 0) {
        return 0;
    }
    if (param_2 >= 3) {
        return 0;
    }
    return *(unsigned char *)(model + param_2 + 0x1e4);
}
