extern char data_020a7630;
extern void func_ov006_020d7dc4(unsigned int *object);
extern unsigned char *func_0202fe34(unsigned char *object);

unsigned int *func_0203ed9c(unsigned int *param_1)
{
    func_0202fe34((unsigned char *)param_1);
    *param_1 = (unsigned int)&data_020a7630;
    *(unsigned char *)(param_1 + 0x2f) = 0;
    *(unsigned char *)((int)param_1 + 0xbd) = 0;
    *(signed char *)((int)param_1 + 0xbe) = -1;
    *(unsigned char *)((int)param_1 + 0xbf) = 0;
    param_1[0x32] = 0xffffffff;
    param_1[0x33] = 0xffffffff;
    func_ov006_020d7dc4(param_1 + 0x37);
    return param_1;
}
