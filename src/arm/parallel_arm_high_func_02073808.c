extern void func_020737bc(unsigned int, unsigned int, unsigned int, unsigned int);

void func_02073808(unsigned int param_1, unsigned int param_2, unsigned int param_3)
{
    void (*fn)(unsigned int, unsigned int, unsigned int, unsigned int);
    fn = func_020737bc;
    fn(param_1, param_2, param_3, 0);
}
