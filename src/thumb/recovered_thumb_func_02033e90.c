extern char data_020a58dc;
extern int func_0200ead8();
extern int func_0200eaec();
extern int func_02033e48();

unsigned int * func_02033e90(unsigned int *param_1)

{
    func_02033e48();
    *param_1 = ((unsigned int)&data_020a58dc);
    param_1[0x55] = 0;
    param_1[0x56] = 0;
    *(unsigned short *)(param_1 + 0x57) = 0;
    *(unsigned short *)((int)param_1 + 0x15e) = 0;
    *(unsigned char *)(param_1 + 0x58) = 0;
    *(unsigned char *)((int)param_1 + 0x161) = 0;
    *(unsigned char *)((int)param_1 + 0x162) = 0;
    *(unsigned char *)((int)param_1 + 0x163) = 6;
    *(unsigned char *)(param_1 + 0x59) = 0;
    param_1[0x5a] = 0;
    func_0200ead8(param_1 + 0x5b);
    func_0200eaec(param_1 + 0x5c);
    param_1[0x5d] = 0;
    return param_1;
}
