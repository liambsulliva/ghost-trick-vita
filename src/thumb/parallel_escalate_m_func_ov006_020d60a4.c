extern int func_ov006_020d793c(unsigned char *, int, unsigned int, unsigned int);
extern void func_ov006_020d78c4(unsigned char *, int, short);
extern void func_ov006_020d7be8(unsigned char *, unsigned char *);
extern unsigned int func_0202f64c(int *);

void func_ov006_020d60a4(unsigned char *param_1, unsigned char *param_2)
{
    unsigned char flag;
    unsigned int shifted;
    unsigned int i;
    int *a;
    int *b;

    flag = param_1[0x25] & 1;
    shifted = ((unsigned int)param_1[0x25] << 23) >> 24;
    func_ov006_020d793c(param_1, 0, flag, shifted);
    *(short *)(param_1 + 0x3dc) = *(short *)(param_1 + 0x3dc) + 0x10;
    if (*(short *)(param_1 + 0x3dc) > 0x180) {
        a = (int *)(param_1 + 0x438 + (1 - flag) * 0x84);
        b = (int *)(param_1 + 0x540 + (1 - flag) * 0x84);
        i = 0;
        do {
            func_0202f64c(a + i);
            func_0202f64c(b + i);
            i++;
        } while (i < 0x21);
        *(short *)(param_1 + 0x3dc) = 0x180;
        *(short *)(param_1 + 0x3de) = 0x30;
        param_1[0x26] = 6;
        func_ov006_020d7be8(param_1, param_2);
    }
    if (flag != 0) {
        func_ov006_020d78c4(param_1, 1, 0x180 - *(short *)(param_1 + 0x3dc));
        func_ov006_020d78c4(param_1, 0, -*(short *)(param_1 + 0x3dc));
    } else {
        func_ov006_020d78c4(param_1, 1, *(short *)(param_1 + 0x3dc));
        func_ov006_020d78c4(param_1, 0, *(short *)(param_1 + 0x3dc) - 0x180);
    }
}
