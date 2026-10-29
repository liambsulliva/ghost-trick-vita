extern char data_020a7668;
extern int func_0202fe34();

unsigned int * func_0203ee00(unsigned int *param_1)

{
    func_0202fe34();
    *param_1 = ((unsigned int)&data_020a7668);
    param_1[0x2f] = 0xffffffff;
    *(unsigned char *)(param_1 + 0x30) = 0;
    *(unsigned char *)((int)param_1 + 0xc1) = 2;
    *(unsigned char *)((int)param_1 + 0xc2) = *(unsigned char *)((int)param_1 + 0xc2) & ~0x1;
    *(unsigned char *)((int)param_1 + 0xc2) = *(unsigned char *)((int)param_1 + 0xc2) & ~0x2;
    *(unsigned char *)((int)param_1 + 0xc2) = *(unsigned char *)((int)param_1 + 0xc2) & ~0x4;
    return param_1;
}
