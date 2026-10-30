void func_020521d4(unsigned int, int, int, int);
extern int func_ov003_020d4818();

void func_ov003_020d5540(unsigned char *param_1)
{
    unsigned int count;
    int value;

    count = *(unsigned int *)(param_1 + 0x174) + 1;
    *(unsigned int *)(param_1 + 0x174) = count;
    if (count > 8) {
        value = 0xf;
        func_020521d4(0x020d5afcu, 0, value, value + 0xf1);
        func_020521d4(0x020d5afcu, 1, value, value + 0xf1);
        param_1[0x168] = 9;
    }
    func_ov003_020d4818(param_1);
}
