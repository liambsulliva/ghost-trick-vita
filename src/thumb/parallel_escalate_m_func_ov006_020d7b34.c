int func_ov006_020d7b34(unsigned char *param_1, int param_2)
{
    unsigned char bit;
    int q;
    short v;
    signed char r;
    int lim;

    bit = param_1[0x25] & 1;
    q = ((signed char *)(param_1 + bit))[0x32];
    v = *(short *)(param_1 + 0x3e6);
    if (v > 0) {
        v = v + 0x70;
    } else {
        v = v - 0x70;
    }
    r = q - v / 128;
    lim = ((signed char *)param_1 + bit)[0x36] - 1;
    if (r > lim) {
        r = lim;
    }
    if (r < 0) {
        r = 0;
    }
    return r;
}
