extern int func_ov006_020d793c(unsigned char *, int, int, int);

void func_ov006_020d6bf8(unsigned char *param_1)
{
    unsigned char flags;

    flags = param_1[0x25];
    if (func_ov006_020d793c(param_1, 1, (unsigned char)(flags & 1), (unsigned char)(flags >> 1)) != 0) {
        param_1[0x26] = 0;
    }
}
