extern void *func_0202a890();

void *func_0202a0ec(int param_1, int param_2, unsigned short param_3, unsigned short param_4)
{
    if (*(unsigned char *)(param_1 + 0x24) == 1) {
        return 0;
    }
    if ((param_2 == 0) || (param_2 == 0x21)) {
        return 0;
    }
    return func_0202a890(param_1, param_2, param_3, 0, param_4);
}
