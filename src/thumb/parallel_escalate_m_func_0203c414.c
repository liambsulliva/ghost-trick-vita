extern int func_02022d70();
extern int data_020a71c0;

unsigned int *func_0203c414(unsigned int *param_1)
{
    unsigned int uVar1;

    func_02022d70(param_1);
    *param_1 = (unsigned int)&data_020a71c0;
    *(unsigned short *)(param_1 + 0x11) = 0;
    *(unsigned short *)((int)param_1 + 0x46) = 0;
    *(unsigned short *)(param_1 + 0x12) = 0;
    *(unsigned short *)((int)param_1 + 0x4a) = 0;
    *(unsigned char *)(param_1 + 0x13) = 0;
    *(unsigned char *)((int)param_1 + 0x4d) = 0;
    *(unsigned char *)((int)param_1 + 0x4e) = 0;
    *(unsigned char *)((int)param_1 + 0x4f) = 0;
    *(unsigned char *)(param_1 + 0x14) = 0;
    *(unsigned char *)((int)param_1 + 0x51) = 0;
    *(unsigned char *)((int)param_1 + 0x52) = 0;
    *(unsigned char *)((int)param_1 + 0x53) = 0;
    *(unsigned char *)(param_1 + 0x15) = 0;
    *(unsigned char *)((int)param_1 + 0x55) = 0;
    *(unsigned char *)((int)param_1 + 0x56) = 0;
    *(unsigned char *)((int)param_1 + 0x57) = 0;
    param_1[8] = 0;
    param_1[9] = 0;
    param_1[10] = 0;
    param_1[0xb] = 0;
    param_1[0xc] = 0;
    param_1[0xd] = 0;
    param_1[0x10] = 0;
    *(unsigned char *)((int)param_1 + 0x58) = 0;
    param_1[0x17] = 0;
    for (uVar1 = 0; uVar1 < 2; uVar1++) {
        param_1[uVar1 + 0xe] = 0;
    }
    return param_1;
}
