void func_02029800(int param_1,int param_2)

{
    if ((param_2 != 0) || (0 < *(int *)(param_1 + 0x58))) {
        *(unsigned short *)(param_1 + 0x4c) = 0x2d;
        *(unsigned short *)(param_1 + 0x4e) = 0;
        *(unsigned char *)(param_1 + 0x54) = 0;
        *(unsigned char *)(param_1 + 0x57) = 0;
        *(unsigned short *)(param_1 + 0x50) = 0;
        *(unsigned int *)(param_1 + 0x58) = 0x1f;
    }
    return;
}
