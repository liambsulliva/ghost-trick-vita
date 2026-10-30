extern int data_020b1748;
void func_02072974(int, int, int, unsigned int, int);
void func_02072ed8(int, int, unsigned int);

void func_020702f4(int param_1, int param_2, unsigned int param_3)
{
    if (data_020b1748 != -1 && param_3 > 48) {
        func_02072974(data_020b1748, param_1, param_2 + 0x7000400, param_3, 1);
        return;
    }
    func_02072ed8(param_1, param_2 + 0x7000400, param_3);
}
