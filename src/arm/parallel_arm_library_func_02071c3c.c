void func_02071b6c(unsigned int, unsigned int);

void func_02071c3c(void)
{
    unsigned int *reg;
    unsigned int value;

    reg = (unsigned int *)0x04001000;
    value = *reg;
    value = value & 0x7fffffff;
    *reg = value;
    func_02071b6c(0x020d0e9c, value);
}
