extern void func_ov006_020d79b4(unsigned char *, int, int, int, int, int);
extern int func_ov006_020d7704(unsigned char *, int, int, int);
extern void func_ov006_020d7108(unsigned char *, int, int);
extern void func_02014ddc(void *, int);

void func_ov006_020d628c(unsigned char *param_1, int param_2)
{
    unsigned char flag;
    signed char t;
    short v;

    flag = param_1[0x25] & 1;
    v = *(short *)(param_1 + 0x3e6);
    if (v >= 8) {
        v = v - 0x10;
        *(short *)(param_1 + 0x3e6) = v;
        if (*(short *)(param_1 + 0x3e6) < 8) {
            *(short *)(param_1 + 0x3e6) = 0;
        }
        func_ov006_020d79b4(param_1, param_2, *(signed char *)(param_1 + 0x2a), 1, 0, *(short *)(param_1 + 0x3e6));
    } else if (v <= -8) {
        v = v + 0x10;
        *(short *)(param_1 + 0x3e6) = v;
        if (*(short *)(param_1 + 0x3e6) > -8) {
            *(short *)(param_1 + 0x3e6) = 0;
        }
        func_ov006_020d79b4(param_1, param_2, *(signed char *)(param_1 + 0x2a), 1, 0, *(short *)(param_1 + 0x3e6));
    } else {
        t = *(signed char *)(param_1 + 0x2a);
        *(signed char *)((int)(param_1 + 0x34) + flag) = t;
        *(signed char *)(param_1 + flag + 0x32) = t;
        func_ov006_020d7704(param_1, param_2, flag, 1);
        param_1[0x2b] = 0x10;
        func_ov006_020d7108(param_1, param_2, 1);
        param_1[0x26] = 0;
    }
    v = *(short *)(param_1 + 0x3e6);
    func_02014ddc(*(void **)(param_1 + 0x660), ((v / 4) + 0x80) << 12);
}
