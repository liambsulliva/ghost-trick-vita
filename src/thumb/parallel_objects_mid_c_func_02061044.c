int func_02061044(int param_1)
{
    if (*(unsigned short *)(param_1 + 0xec) == 0) {
        return 0;
    }
    if ((((unsigned int)*(unsigned char *)(param_1 + 0x7c) << 26) >> 31) == 0) {
        return 0;
    }
    if (*(unsigned char *)(param_1 + 0x7e) != 0) {
        return 1;
    }
    return 0;
}
