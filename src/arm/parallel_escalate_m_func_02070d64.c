extern int data_020b1748;
void func_02072bc8(int, int, int, unsigned int, int, int, int);
void func_02072ed8(int, int, unsigned int);

void func_02070d64(int param_1, int param_2, unsigned int param_3)
{
    if (data_020b1748 != -1) {
        func_02072bc8(data_020b1748, param_1, param_2 + 0x6898000, param_3, 0, 0, 1);
        return;
    }
    func_02072ed8(param_1, param_2 + 0x6898000, param_3);
}
