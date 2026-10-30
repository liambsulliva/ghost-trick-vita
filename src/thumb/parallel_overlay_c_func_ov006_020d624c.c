extern int func_ov006_020d79b4();
extern unsigned char func_ov006_020d7b88(unsigned char *, int);

void func_ov006_020d624c(unsigned char *param_1, int param_2)
{
    unsigned char bit;
    signed char *q;

    bit = param_1[0x25] & 1;
    q = (signed char *)(param_1 + bit);
    if (func_ov006_020d79b4(param_1, param_2, q[0x32], 0, 0, 0) != 0) {
        param_1[0x2a] = func_ov006_020d7b88(param_1, param_2);
        param_1[0x26] = 0xb;
    }
}
