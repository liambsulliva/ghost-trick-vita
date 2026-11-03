extern void func_02078068(unsigned int param_1, unsigned int *param_2);
extern unsigned int func_02078584(unsigned int callback, unsigned int *result);
extern void func_02077fe4(void);

unsigned int func_020785b0(void)
{
    unsigned int result;
    unsigned int r;

    r = func_02078584((unsigned int)func_02078068, &result);
    if (r != 0) {
        return r;
    }
    *(unsigned int *)0x020b17a8 = 0xc;
    func_02077fe4();
    *(unsigned int *)0x020b17a8 = 2;
    return result;
}
