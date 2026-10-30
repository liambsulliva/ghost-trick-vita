extern void func_0202f524(unsigned int *param_1);
extern void func_02042718(unsigned int *param_1);

extern int data_020aeafc;

unsigned int *func_0205a850(unsigned int *param_1)
{
    unsigned int i;

    func_0202f524(param_1);
    *param_1 = (unsigned int)&data_020aeafc;
    *(unsigned char *)((int)param_1 + 0x2e) = 0;
    *(unsigned char *)((int)param_1 + 0x2f) = 0;
    *(unsigned char *)(param_1 + 0xc) = 0;
    *(unsigned char *)((int)param_1 + 0x31) = 0;
    *(unsigned char *)((int)param_1 + 0x32) = 0;
    *(unsigned char *)((int)param_1 + 0x33) = 0;
    param_1[0x31] = 0;
    *(unsigned short *)(param_1 + 10) = 0;
    *(unsigned short *)((int)param_1 + 0x2a) = 3;
    *(unsigned short *)(param_1 + 0xb) = 0;
    param_1[0xd] = 0;
    param_1[0xe] = 0;
    param_1[0xf] = 0;
    param_1[0x10] = 0;
    func_02042718(param_1 + 0x32);
    param_1[0x47] = 0;
    *(signed char *)(param_1 + 0x48) = -1;
    i = 0;
    do {
        param_1[i + 0x11] = 0;
        i++;
    } while (i < 0x20);
    return param_1;
}
