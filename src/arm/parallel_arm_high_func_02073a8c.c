extern void func_02073ab4(unsigned int param_1, unsigned int param_2, unsigned int param_3);

void func_02073a8c(unsigned int param_1, unsigned int param_2, ...)
{
    unsigned int ap;

    ap = ((unsigned int)&param_2 & ~3) + 4;
    func_02073ab4(param_1, param_2, ap);
}
