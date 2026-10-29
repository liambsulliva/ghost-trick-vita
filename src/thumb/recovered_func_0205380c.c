extern int func_02052cec();

unsigned int * func_0205380c(unsigned int *param_1)

{
    func_02052cec();
    *param_1 = ((unsigned int)0x020acdccu);
    *(unsigned char *)(param_1 + 0x55) = 0;
    param_1[0x56] = 0xffffffff;
    return param_1;
}
