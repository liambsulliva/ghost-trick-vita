#pragma optimization_level 0

extern void *func_0202f25c(void);
extern unsigned int func_0202f24c(unsigned char *object);
extern unsigned int func_ov004_020d6f8c(int param_1, int param_2);
extern unsigned int func_ov004_020d6fc0(int param_1, int param_2);
extern unsigned int func_ov004_020d6ff4(int param_1, int param_2);
extern int func_0206ef28(int numer, int denom);

int func_ov004_020d6f34(void)
{
    int a;
    int b;
    int c;

    if (func_0202f24c(func_0202f25c()) != 0) {
        a = func_ov004_020d6f8c((int)func_0202f25c(), -1);
        b = func_ov004_020d6fc0((int)func_0202f25c(), -1);
        c = func_ov004_020d6ff4((int)func_0202f25c(), -1);
        a = func_0206ef28(0x3c000, a);
        c = (a >> 12) * (c - b);
        return c;
    }
    return 0;
}
