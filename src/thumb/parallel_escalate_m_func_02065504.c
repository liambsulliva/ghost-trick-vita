extern int data_020b02c4;

unsigned char *func_02065504(unsigned char *param_1, unsigned char param_2)
{
    *(int *)param_1 = (int)&data_020b02c4;
    *(int *)(param_1 + 4) = 0;
    *(int *)(param_1 + 8) = 0;
    *(int *)(param_1 + 0xc) = 0;
    *(int *)(param_1 + 0x10) = 0;
    param_1[0x14] = param_2;
    *(int *)(param_1 + 0x18) = 0;
    *(int *)(param_1 + 0x1c) = 0;
    param_1[0x20] = 0;
    return param_1;
}
