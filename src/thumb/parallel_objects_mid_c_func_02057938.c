int func_02057938(int param_1, int param_2, int param_3)
{
    int dx;
    int dy;
    int hx;
    int hy;
    int w;

    hx = (int)*(short *)(param_1 + 0x7c) >> 5;
    dx = (int)*(short *)(param_1 + 0x8c) >> 5;
    dy = (param_2 + -0x80) - ((*(int *)(param_1 + 0x28) >> 0xc) + ((hx + dx) >> 1));
    hy = (0x60 - param_3) - ((*(int *)(param_1 + 0x2c) >> 0xc) + ((((int)*(short *)(param_1 + 0x7e) >> 5) + ((int)*(short *)(param_1 + 0x8e) >> 5)) >> 1));
    w = dx - hx;
    if (dy * dy + hy * hy - 0x40 > (w * w >> 2)) {
        return 0;
    }
    return 1;
}
