extern void func_02014de4();

int func_ov006_020d793c(unsigned char *param_1, int param_2, int param_3, int param_4)
{
    int r6;

    r6 = (*(short *)(param_1 + 0x3de) + 0x60) << 12;
    func_02014de4(*(unsigned int *)(param_1 + 0x410), r6);
    if (param_4 == 0) {
        if (!(param_1[0x26] == 5 && param_2 == 0)) {
            if (*(unsigned int *)(param_1 + 0x414) != 0) {
                func_02014de4(*(unsigned int *)(param_1 + 0x414), r6);
            }
        }
    }
    if (param_2 != 0) {
        *(short *)(param_1 + 0x3de) = *(short *)(param_1 + 0x3de) - 3;
        if (*(short *)(param_1 + 0x3de) >= 0) {
            return 0;
        }
        *(short *)(param_1 + 0x3de) = 0;
        return 1;
    }
    *(short *)(param_1 + 0x3de) = *(short *)(param_1 + 0x3de) + 3;
    if (*(short *)(param_1 + 0x3de) <= 0x30) {
        return 0;
    }
    *(short *)(param_1 + 0x3de) = 0x30;
    return 1;
}
