extern int func_02007aa8();
extern int func_0200ca60();
extern char data_020a4a14;

unsigned int *func_02023c88(unsigned int *param_1)
{
    unsigned int uVar1;

    func_02007aa8(param_1);
    param_1[0] = (unsigned int)&data_020a4a14;
    uVar1 = param_1[2] & 0xff000000;
    param_1[1] = 0;
    param_1[2] = 0x7fffffff & uVar1;
    param_1[3] = 0;
    func_0200ca60(param_1 + 4, 0);
    param_1[4] = 0;
    return param_1;
}
