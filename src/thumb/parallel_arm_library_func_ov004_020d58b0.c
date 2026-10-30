extern void func_ov004_020d5950(unsigned int *);
extern void func_02073ed4(unsigned int *, int, int, unsigned int *, int, int);
extern void func_02074188(unsigned int *);

void func_ov004_020d58b0(int param_1, int param_2, int param_3, int param_4)
{
    int bits;

    *(int *)(param_1 + 0x9c4) = param_2;
    *(int *)(param_1 + 0x9c8) = param_3;
    if (param_2 != 0 || param_3 != 0) {
        if (param_2 == 0 || *(int *)(param_1 + 0x9c8) == 0) {
            bits = *(unsigned char *)(param_1 + 0xaa8);
            bits &= ~1;
            bits |= 1;
            *(unsigned char *)(param_1 + 0xaa8) = bits;
            if (param_4 != -1) {
                if ((param_4 & 1) != 0) {
                    *(unsigned char *)(param_1 + 0xaa8) |= 4;
                }
                if ((param_4 & 2) != 0) {
                    *(unsigned char *)(param_1 + 0xaa8) |= 8;
                }
                if ((param_4 & 4) != 0) {
                    *(unsigned char *)(param_1 + 0xaa8) |= 0x10;
                }
            }
            func_ov004_020d5950((unsigned int *)(param_1 + 0x9bc));
            func_02073ed4((unsigned int *)(param_1 + 0x8fc), 0x020d6bd1, 0, (unsigned int *)(param_1 + 0x8fc), 0x800, 0x12);
            func_02074188((unsigned int *)(param_1 + 0x8fc));
        }
    }
}
