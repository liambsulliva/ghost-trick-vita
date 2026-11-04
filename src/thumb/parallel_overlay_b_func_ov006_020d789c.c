void func_ov006_020d789c(unsigned char *param_1, int param_2, int param_3)
{
    short *reg = (short *)0x020c9e88;
    *(short *)(param_1 + 0x3d8) = param_2;
    *(short *)(param_1 + 0x3da) = param_3;
    reg[0x16] = *(short *)(param_1 + 0x3d8) & 0x1ff;
    reg[0x17] = *(short *)(param_1 + 0x3da) & 0x1ff;
}
