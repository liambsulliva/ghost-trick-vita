extern int func_02022d70();

unsigned int * func_02035c3c(unsigned int *param_1)

{
    func_02022d70();
    *param_1 = ((unsigned int)0x020a60f8u);
    *(unsigned char *)(param_1 + 0x5a) = 0;
    *(unsigned char *)((int)param_1 + 0x169) = 0;
    *(unsigned char *)((int)param_1 + 0x16a) = 0;
    *(unsigned char *)((int)param_1 + 0x16b) = 0;
    *(unsigned char *)(param_1 + 0x5b) = 0;
    param_1[0x5d] = 0;
    param_1[0x5e] = 0;
    param_1[0x5f] = 0;
    param_1[0x61] = 0;
    return param_1;
}
