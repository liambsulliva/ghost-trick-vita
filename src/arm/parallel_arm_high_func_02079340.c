extern int func_0207936c();

void func_02079340(unsigned char *param_1, unsigned char *param_2, ...)
{
    unsigned int ap;

    ap = ((unsigned int)&param_2 & ~3) + 4;
    func_0207936c(param_1, 0x7fffffff, param_2, ap);
}
