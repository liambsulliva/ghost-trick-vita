extern int func_02053be8();

unsigned int * func_02053d14(unsigned int *param_1)

{
    func_02053be8();
    *param_1 = ((unsigned int)0x020ad150u);
    *(unsigned char *)(param_1 + 0x55) = 0;
    param_1[0x56] = 0xffffffff;
    return param_1;
}
