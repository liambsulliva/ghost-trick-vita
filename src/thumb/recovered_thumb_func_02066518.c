void func_02066518(int param_1,unsigned short param_2,unsigned int param_3,unsigned int param_4)

{
    *(unsigned int *)(param_1 + 0x94) = *(unsigned int *)(param_1 + 0x34);
    *(unsigned int *)(param_1 + 0x98) = *(unsigned int *)(param_1 + 0x38);
    *(unsigned int *)(param_1 + 0x9c) = param_3;
    *(unsigned int *)(param_1 + 0xa0) = param_4;
    *(unsigned short *)(param_1 + 0xa4) = param_2;
    *(unsigned short *)(param_1 + 0xa6) = 0;
    return;
}
