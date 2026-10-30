extern int func_ov006_020d7704(unsigned char *, int, unsigned char, int);
extern void func_ov006_020d78c4(unsigned char *, int, short);

void func_ov006_020d6050(unsigned char *param_1, int param_2)
{
    unsigned char bit;

    bit = param_1[0x25] & 1;
    func_ov006_020d7704(param_1, param_2, bit, 0);
    if (bit != 0) {
        func_ov006_020d78c4(param_1, 1, (short)(0x180 - *(short *)(param_1 + 0x3dc)));
    }
    else {
        func_ov006_020d78c4(param_1, 0, (short)(*(short *)(param_1 + 0x3dc) - 0x180));
    }
    param_1[0x26] = 8;
}
