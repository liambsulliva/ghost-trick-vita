extern int func_ov006_020d7b34(unsigned char *, int);
extern int func_ov006_020d7594(unsigned char *, unsigned int, int, int);
extern void func_02014ddc(void *, int);

int func_ov006_020d7b88(unsigned char *param_1, int param_2)
{
    unsigned char bit;
    int r7;
    int r6;
    int s;
    short t;

    bit = param_1[0x25] & 1;
    r7 = func_ov006_020d7b34(param_1, param_2);
    r6 = func_ov006_020d7594(param_1, bit, r7, r7);
    s = func_ov006_020d7594(param_1, bit, r7, ((signed char *)(param_1 + bit))[0x34]);
    *(short *)(param_1 + 0x3e6) = *(short *)(param_1 + 0x3e6) + r6 - s;
    t = *(short *)(param_1 + 0x3e6);
    func_02014ddc(*(void **)(param_1 + 0x660), ((t / 4) + 0x80) << 12);
    return r7;
}
