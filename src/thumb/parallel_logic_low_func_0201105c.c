extern int func_0204fec0();

int func_0201105c(unsigned char *param_1, int param_2, int param_3)
{
    int field;

    field = *(int *)(param_1 + 0xfb8);
    if (field != 0) {
        return func_0204fec0(field, param_2, param_3);
    }
    return 0;
}
