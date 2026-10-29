extern int func_02007aa8();
extern int func_0200d7e4();
extern int func_02050ac4();

unsigned int * func_0205083c(unsigned int *param_1)

{
    func_02007aa8();
    *param_1 = ((unsigned int)0x020abcf8u);
    func_0200d7e4(param_1 + 1);
    func_02050ac4(param_1 + 5);
    *(unsigned char *)(param_1 + 0x16) = 0;
    *(unsigned char *)((int)param_1 + 0x59) = 0;
    *(unsigned char *)((int)param_1 + 0x5a) = 0;
    param_1[0x14] = 0;
    param_1[0x15] = 0;
    return param_1;
}
