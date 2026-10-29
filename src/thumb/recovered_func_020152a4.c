void func_020152a4(int param_1,int param_2,unsigned char param_3,unsigned short param_4,unsigned short param_5)

{
    if (param_2 == 0) {
        param_2 = ((int)0x0000010eu);
    }
    *(int *)(param_1 + 0x2c) = param_2;
    *(unsigned char *)(param_1 + 0x38) = param_3;
    *(unsigned short *)(param_1 + 0x30) = param_4;
    *(unsigned short *)(param_1 + 0x32) = param_5;
    return;
}
