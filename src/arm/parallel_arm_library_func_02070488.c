extern int data_020b1748;
int func_0206fcdc(void);
void func_02072a18(int, int, int, unsigned int, int);
void func_02072ea8(int, int, unsigned int);

void func_02070488(int param_1, int param_2, unsigned int param_3, int param_4)
{
    int iVar1;

    iVar1 = func_0206fcdc();
    if (data_020b1748 != -1 && param_3 > 28) {
        func_02072a18(data_020b1748, param_1, iVar1 + param_2, param_3, 1);
        return;
    }
    func_02072ea8(param_1, iVar1 + param_2, param_3);
}
