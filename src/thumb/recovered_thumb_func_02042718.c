extern char data_020a7e54;
extern int func_02007aa8();

unsigned int * func_02042718(unsigned int *param_1)

{
    func_02007aa8();
    *param_1 = ((unsigned int)&data_020a7e54);
    param_1[1] = 0xffffffff;
    param_1[2] = 0xffffffff;
    param_1[3] = 0xffffffff;
    param_1[4] = 0xffffffff;
    param_1[5] = 0xffffffff;
    *(unsigned char *)(param_1 + 9) = 0;
    param_1[0xc] = 0x100;
    *(unsigned char *)(param_1 + 0xe) = 0;
    *(unsigned char *)((int)param_1 + 0x39) = 0;
    *(unsigned char *)((int)param_1 + 0x3a) = 1;
    param_1[6] = 0;
    param_1[7] = 0;
    param_1[8] = 0;
    param_1[10] = 0;
    param_1[0xb] = 0;
    param_1[0xd] = 0;
    param_1[0xf] = 0;
    param_1[0x10] = 0;
    param_1[0x11] = 0;
    param_1[0x12] = 0;
    param_1[0x13] = 0;
    param_1[0x14] = 0;
    return param_1;
}
