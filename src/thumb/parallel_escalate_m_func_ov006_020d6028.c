extern int func_ov006_020d793c(unsigned char *, int, unsigned char, unsigned int);

void func_ov006_020d6028(unsigned char *param_1)
{
    unsigned int flags;
    unsigned char bit;
    unsigned int shifted;

    flags = param_1[0x25];
    bit = flags & 1;
    shifted = (flags << 23) >> 24;
    if (func_ov006_020d793c(param_1, 1, bit, shifted) != 0) {
        param_1[0x26] = 0;
    }
}
