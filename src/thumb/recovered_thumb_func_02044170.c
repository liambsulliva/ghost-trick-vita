extern char data_020a7ecc;
extern int func_02056ac8();

unsigned int * func_02044170(unsigned int *param_1,unsigned int param_2)

{
    func_02056ac8();
    *param_1 = ((unsigned int)&data_020a7ecc);
    param_1[0x32] = param_2;
    param_1[0x33] = 0;
    param_1[0x34] = 0;
    param_1[0x35] = 0;
    param_1[0x36] = 0;
    param_1[0x37] = 0;
    param_1[0x38] = 0;
    *(unsigned char *)(param_1 + 0x39) = 0;
    return param_1;
}
