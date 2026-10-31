extern void func_02072db0();
extern void func_02072e3c();
extern void func_01ff80f4();

void func_02073254(int param_1, unsigned int param_2, unsigned int param_3, int param_4)
{
    volatile unsigned int *reg;

    func_02072db0(param_1, 0xffffffff);
    func_02072e3c(param_1, param_2, param_4, 0x1000000);
    if (param_4 == 0) {
        return;
    }
    reg = (volatile unsigned int *)((param_1 * 3 + 2) * 4 + 0x40000b0);
    do {
    } while ((*reg & 0x80000000) != 0);
    func_01ff80f4(param_1, param_2, param_3, 0xaf000001, 0);
}
