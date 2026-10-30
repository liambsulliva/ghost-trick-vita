extern unsigned int func_0206f28c();

int func_020578c0(int param_1, int param_2, int param_3)
{
    int x_lo;
    int x_hi;
    int y_hi;
    int y_lo;
    int x;
    int y;

    x_lo = func_0206f28c((int)*(short *)(param_1 + 0x7c) << 7, *(int *)(param_1 + 0x34));
    y_hi = func_0206f28c((int)*(short *)(param_1 + 0x7e) << 7, *(int *)(param_1 + 0x38));
    x_hi = func_0206f28c((int)*(short *)(param_1 + 0x8c) << 7, *(int *)(param_1 + 0x34));
    y_lo = func_0206f28c((int)*(short *)(param_1 + 0x8e) << 7, *(int *)(param_1 + 0x38));
    x = ((param_2 - 0x80) << 12) - *(int *)(param_1 + 0x28);
    y = ((0x60 - param_3) << 12) - *(int *)(param_1 + 0x2c);
    if (x < x_lo) {
        return 0;
    }
    if (x > x_hi) {
        return 0;
    }
    if (y < y_lo) {
        return 0;
    }
    if (y > y_hi) {
        return 0;
    }
    return 1;
}
