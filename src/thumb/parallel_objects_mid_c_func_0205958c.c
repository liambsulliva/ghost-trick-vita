extern int func_020082d8();
extern char data_020a46c8;

void func_0205958c(int param_1, int param_2, unsigned int param_3)
{
    unsigned int index;

    if (param_3 < 4) {
        func_020082d8(param_1, param_2 + 0xe4 + param_3 * 0xc);
        return;
    }
    if (param_3 >= 0xa) {
        index = param_3 - 0xa;
        if (index < *(unsigned char *)(param_2 + 0xbe)) {
            func_020082d8(param_1, *(int *)(param_2 + index * 4 + 0xc4) + 8);
            return;
        }
    }
    func_020082d8(param_1, 0x020a46c8);
    return;
}
