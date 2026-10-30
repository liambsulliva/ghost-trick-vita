void func_020521d4(unsigned int, int, int, int);
extern int func_02008444();
extern int func_02031fd0();
extern int func_020125d4();
extern int func_ov003_020d4818();

void func_ov003_020d55c8(unsigned char *param_1)
{
    unsigned int count;
    int value;
    unsigned char *base;

    count = *(unsigned int *)(param_1 + 0x174) + 1;
    *(unsigned int *)(param_1 + 0x174) = count;
    if (count > 8) {
        value = 0xf;
        func_020521d4(0x020d5afcu, 0, value, value + 0xf1);
        func_020521d4(0x020d5afcu, 1, value, value + 0xf1);
        if (func_02031fd0(func_02008444()) != 0) {
            func_020125d4(func_02008444(), 0);
            base = param_1 + *(unsigned int *)(param_1 + 0x178);
            func_020125d4(func_02008444(), base[0x188]);
        }
        param_1[0x168] = 11;
    }
    func_ov003_020d4818(param_1);
}
