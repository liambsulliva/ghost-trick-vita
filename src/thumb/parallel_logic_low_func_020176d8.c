extern const char data_02084a98;

int func_020176d8(unsigned char *param_1, int param_2, int param_3)
{
    unsigned int uVar1;

    uVar1 = *(unsigned short *)(param_1 + 0x38);
    if (uVar1 == 1) {
        return *(short *)((char *)&data_02084a98 + param_3 * 18);
    }
    if (uVar1 == 2) {
        return *(short *)((char *)&data_02084a98 + 2 + param_3 * 18);
    }
    return *(short *)((char *)&data_02084a98 + 4 + param_3 * 18);
}
