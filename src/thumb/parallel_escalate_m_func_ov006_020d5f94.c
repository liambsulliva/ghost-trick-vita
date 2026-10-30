extern int func_ov006_020d7704(unsigned char *, int, unsigned char, int);
extern void func_ov006_020d7be8(unsigned char *, int);

void func_ov006_020d5f94(unsigned char *param_1, int param_2)
{
    unsigned char bit;

    bit = param_1[0x25] & 1;
    func_ov006_020d7704(param_1, param_2, bit, 1);
    func_ov006_020d7be8(param_1, param_2);
    param_1[0x26] = 0;
}
