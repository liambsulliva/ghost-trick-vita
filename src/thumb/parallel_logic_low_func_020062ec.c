struct State
{
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern unsigned int func_0207b8a0();
extern void func_02005654();
extern void func_02005cec();

void func_020062ec(int param_1, unsigned int param_2, int param_3)
{
    unsigned int uVar3;
    uVar3 = func_0207b8a0(*(int *)(param_1 + 0x64) * 1000, *(unsigned short *)(param_1 + 0xa4));
    func_02005654(param_2);
    func_02005cec(param_2, uVar3, 0, 0xffffffff, 1, *(unsigned char *)(param_1 + 0xf2));
    if ((*(unsigned int *)(data_020b18a0.data + param_3 * 0x24 + 0x4bc0) & 2) != 0) {
        *(unsigned char *)(param_1 + 0x7d) = 8;
        *(unsigned int *)(param_1 + 0x84) = *(unsigned int *)(data_020b18a0.data + param_3 * 0x24 + 0x4bc0 + 0x18);
    }
}
