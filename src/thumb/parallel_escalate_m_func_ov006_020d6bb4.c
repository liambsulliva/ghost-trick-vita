extern int func_ov006_020d793c(unsigned char *, int, unsigned char, unsigned char);
extern void func_ov006_020d70f0(unsigned char *, int);

void func_ov006_020d6bb4(unsigned char *param_1, int param_2)
{
    if (func_ov006_020d793c(param_1, 0, param_1[0x25] & 1, param_1[0x25] >> 1) != 0) {
        param_1[0x25] = param_1[0x25] - 2;
        param_1[0x26] = 6;
        func_ov006_020d70f0(param_1, param_2);
    }
}
