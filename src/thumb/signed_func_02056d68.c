extern unsigned long long func_0207b694();

void func_02056d68(int param_1,int param_2,unsigned char param_3)

{
    unsigned long long uVar1;

    if (param_2 == 0) {
        param_2 = 1;
    }
    *(short *)(param_1 + 0xc6) = (short)param_2;
    *(unsigned char *)(param_1 + 200) = param_3;
    uVar1 = func_0207b694(((int)*(signed char *)(param_1 + 200) - (int)*(signed char *)(param_1 + 0x44)) * 0x1000,
                                                (int)*(short *)(param_1 + 0xc6));
    *(int *)(param_1 + 0xd0) = (int)uVar1;
    return;
}
