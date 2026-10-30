extern int func_0205b9a8();

void func_0205b228(int param_1)
{
    signed char index;
    int result;

    index = *(signed char *)(param_1 + 0x120);
    result = func_0205b9a8(*(int *)(param_1 + index * 4 + 0x44), *(unsigned int *)(param_1 + 0xc4), (unsigned char)index);
    *(unsigned short *)(param_1 + 0x28) = *(unsigned short *)(param_1 + 0x28) + result;
}
