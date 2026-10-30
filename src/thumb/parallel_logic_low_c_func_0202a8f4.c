void func_0202a8f4(void *param_1, int param_2, int param_3, unsigned short param_4, unsigned short param_5, short param_6, int param_7)
{
    unsigned short *pVar1;

    pVar1 = &param_5;
    *(unsigned short *)((int)param_1 + 0x2c) = param_3;
    *(unsigned short *)((int)param_1 + 0x2e) = param_4;
    *(int *)((int)param_1 + 0x28) = param_2;
    *(unsigned short *)((int)param_1 + 0x30) = *pVar1;
    *(short *)((int)param_1 + 0x32) = ((short *)pVar1)[2];
    *(int *)((int)param_1 + 0x34) = param_7;
}
