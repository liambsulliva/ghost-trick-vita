extern char data_020b15cc[];
extern unsigned int *func_0202fe34(unsigned int *param_1);
extern unsigned int *func_02067df4(unsigned int *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5, unsigned int param_6);
extern void func_0200b518(unsigned char *object, int index, int first, int second);

unsigned int *func_02067c88(unsigned int *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5, unsigned int param_6, unsigned int param_7, unsigned int param_8)
{
    func_0202fe34(param_1);
    *param_1 = (unsigned int)data_020b15cc;
    func_02067df4(param_1 + 0x2f, (unsigned int)param_1, param_2, param_3, param_4, param_5);
    param_1[0x37] = param_6;
    param_1[0x38] = param_7;
    param_1[0x39] = param_8;
    func_0200b518((unsigned char *)(param_1 + 10), 1, 0x80000000, 0x80000000);
    return param_1;
}
