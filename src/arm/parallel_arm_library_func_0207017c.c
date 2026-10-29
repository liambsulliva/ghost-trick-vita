extern int data_020b1748;
void func_02072a18(int, int, int, unsigned int, int);
void func_02072ea8(int, int, unsigned int);

void func_0207017c(int param_1, int param_2, unsigned int param_3)
{
    if (data_020b1748 != -1 && param_3 > 28) {
        func_02072a18(data_020b1748, param_1, param_2 + 0x5000400, param_3, 1);
        return;
    }
    func_02072ea8(param_1, param_2 + 0x5000400, param_3);
}
