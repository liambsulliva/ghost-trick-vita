struct CoderInit {
    unsigned int bits;
    int start;
};

void func_0205ea04(int param_1, unsigned int param_2, int param_3, unsigned int param_4, int param_5, unsigned int param_6, int param_7, int param_8, struct CoderInit s)
{
    *(unsigned int *)(param_1 + 0x728) = param_2;
    *(unsigned int *)(param_1 + 0x738) = param_4;
    *(unsigned int *)(param_1 + 0x748) = param_6;
    *(unsigned int *)(param_1 + 0x75c) = s.bits;
    *(int *)(param_1 + 0x758) = param_8;
    *(int *)(param_1 + 0x73c) = (1 << param_4) - 1;
    *(int *)(param_1 + 0x740) = 0;
    *(int *)(param_1 + 0x744) = param_5;
    *(int *)(param_1 + 0x72c) = (1 << param_2) - 1;
    *(int *)(param_1 + 0x730) = 1 << (param_2 - 1);
    *(int *)(param_1 + 0x734) = param_3;
    *(int *)(param_1 + 0x74c) = (1 << param_6) - 1;
    *(int *)(param_1 + 0x750) = 0;
    *(int *)(param_1 + 0x754) = param_7;
    *(int *)(param_1 + 0x760) = (1 << s.bits) - 1;
    *(int *)(param_1 + 0x764) = 1 << (s.bits - 1);
    *(int *)(param_1 + 0x768) = s.start;
    *(int *)(param_1 + 0x76c) = 1 << (s.bits - 1);
    *(int *)(param_1 + 0x770) = s.start;
    *(int *)(param_1 + 0x774) = (1 << (s.bits - 1)) - 1;
    *(int *)(param_1 + 0x778) = 1 << (s.bits - 2);
    *(int *)(param_1 + 0x77c) = s.start + 2;
    *(int *)(param_1 + 0x780) = (1 << (s.bits - 1)) - 1;
    *(int *)(param_1 + 0x784) = 0;
    *(int *)(param_1 + 0x788) = s.start;
    return;
}
