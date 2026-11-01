extern void func_020664e0(int param_1, unsigned char param_2, unsigned char param_3, unsigned char param_4, int param_5);
void func_02066498(int param_1, int param_2, int param_3, int param_4, signed char param_5)
{
    int object;
    object = *(int *)(param_1 + 0xc8);
    if (object != 0) {
        func_020664e0(object, param_2, param_3, param_4, (param_5 * 100 / 31) << 12);
    }
    *(signed char *)(param_1 + 0x44) = param_5;
}
