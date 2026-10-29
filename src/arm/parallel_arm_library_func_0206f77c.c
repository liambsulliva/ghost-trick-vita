void func_02071e50(int, unsigned int *);

void func_0206f77c(int param_1)
{
    unsigned int *reg;

    reg = (unsigned int *)0x04000400;
    *reg = 23;
    func_02071e50(param_1, reg);
}
