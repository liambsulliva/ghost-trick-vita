extern int func_02008444();
extern int func_0200fe74();

void func_ov003_020d5470(unsigned char *param_1)
{
    unsigned int count;

    count = *(unsigned int *)(param_1 + 0x174) + 1;
    *(unsigned int *)(param_1 + 0x174) = count;
    if (count >= 8) {
        func_02008444();
        func_0200fe74();
        *(unsigned int *)(param_1 + 0x174) = 0;
        param_1[0x168] = 7;
    }
}
