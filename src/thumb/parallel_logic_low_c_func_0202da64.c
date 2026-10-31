extern int func_0202cbe8();
extern char data_020a5240[];

unsigned int *func_0202da64(unsigned int *param_1)
{
    func_0202cbe8(param_1);
    *param_1 = (unsigned int)data_020a5240;
    param_1[0x51] = 0;
    param_1[0x53] = 0;
    param_1[0x54] = 0;
    param_1[0x55] = 0;
    param_1[0x56] = 0;
    param_1[0x62] = 0;
    *(unsigned char *)(param_1 + 99) = 0;
    param_1[0x7a] = 0;
    param_1[0x7c] = 0;
    param_1[0x20] = (unsigned int)param_1;
    return param_1;
}
