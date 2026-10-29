void func_02066500(int param_1,unsigned short param_2)

{
    *(unsigned short *)(param_1 + 0x8e) = param_2;
    *(unsigned short *)(param_1 + 0x90) = 0;
    *(unsigned char *)(param_1 + 0x8c) = 1;
    return;
}
