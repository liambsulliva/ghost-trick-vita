void func_02071b6c(unsigned int, unsigned int);

void func_02071bc8(void)
{
    unsigned int *reg;
    unsigned int value;

    reg = (unsigned int *)0x04000000;
    value = *reg;
    value = value & 0x7fffffff;
    *reg = value;
    func_02071b6c(0x020d0e94, value);
}
