unsigned int func_0203add8(char *param_1)
{
    unsigned short value;
    unsigned char whole;

    value = ((unsigned int)*(unsigned short *)(param_1 + 0xda) << 4) / 0x1555;
    whole = value >> 4;
    if (value % 16 > 7) {
        whole++;
    }
    whole += 3;
    if (whole > 11) {
        whole -= 12;
    }
    return whole;
}
