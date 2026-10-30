void func_02072f8c(int, unsigned int *);

void func_0206f7b4(int param_1)
{
    unsigned int *reg;

    reg = (unsigned int *)0x04000400;
    *reg = 26;
    func_02072f8c(param_1, reg);
}
