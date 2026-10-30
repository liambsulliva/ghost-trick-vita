typedef struct {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
} Flags02059834;

void func_02059834(int param_1, int param_2, int param_3)
{
    Flags02059834 *flags;

    flags = (Flags02059834 *)(param_1 + 0x140);
    if (flags->b4) {
        *(short *)(param_1 + 0x14c) = *(short *)(param_1 + 0x14c) + param_2;
        *(short *)(param_1 + 0x14e) = *(short *)(param_1 + 0x14e) + param_3;
    } else if (!flags->b7) {
        flags->b7 = 1;
        *(short *)(param_1 + 0x150) = param_2;
        *(short *)(param_1 + 0x152) = param_3;
    } else {
        *(short *)(param_1 + 0x150) = *(short *)(param_1 + 0x150) + param_2;
        *(short *)(param_1 + 0x152) = *(short *)(param_1 + 0x152) + param_3;
    }
}
