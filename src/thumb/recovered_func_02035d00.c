extern int func_02035c3c();

unsigned int * func_02035d00(unsigned int *param_1)

{
    func_02035c3c();
    *param_1 = ((unsigned int)0x020a60bcu);
    *(unsigned char *)(param_1 + 0x5a) = 3;
    *(unsigned char *)((int)param_1 + 0x169) = 1;
    return param_1;
}
