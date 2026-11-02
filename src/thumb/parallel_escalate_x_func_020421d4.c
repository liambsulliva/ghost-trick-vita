void func_020421d4(unsigned char *param_1, unsigned char param_2, unsigned short param_3, int param_4, int param_5)
{
    param_1[0] = 1;
    param_1[0x14] = param_2;
    *(unsigned short *)(param_1 + 0xc) = param_3;
    *(int *)(param_1 + 8) = param_4;
    *(int *)(param_1 + 0x10) = param_5;
}
