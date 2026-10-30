extern int data_020b1748;
int func_0206ff74(void);
void func_02072974(int, int, int, unsigned int, int);
void func_02072ed8(int, int, unsigned int);

void func_02070808(int param_1, int param_2, unsigned int param_3, int param_4)
{
    int iVar1;

    iVar1 = func_0206ff74();
    if (data_020b1748 != -1 && param_3 > 48) {
        func_02072974(data_020b1748, param_1, iVar1 + param_2, param_3, 1);
        return;
    }
    func_02072ed8(param_1, iVar1 + param_2, param_3);
}
