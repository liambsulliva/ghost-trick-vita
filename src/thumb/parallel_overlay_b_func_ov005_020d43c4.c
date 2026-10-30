extern int func_ov005_020d4420();

int func_ov005_020d43c4(unsigned char *param_1)
{
    unsigned int *reg;
    signed char i;

    reg = (unsigned int *)0x020c9d50;
    if ((reg[1] & 0x1000) != 0) {
        return 1;
    }
    if (((reg[1] & 0x40) != 0) && (*(int *)(param_1 + 0x30) != 0)) {
        param_1[0x50] = 4;
        return 1;
    }
    i = 0;
    while (i < 5) {
        if (*(int *)(param_1 + i * 4 + 0x20) != 0 && func_ov005_020d4420() != 0) {
            param_1[0x50] = i;
            return 1;
        }
        i = i + 1;
    }
    return 0;
}
