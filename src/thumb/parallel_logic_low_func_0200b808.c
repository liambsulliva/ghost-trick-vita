extern int func_0200b848();
extern int func_0200b854();
extern unsigned char *func_02008860();
extern void func_0200c64c();

int func_0200b808(char *param_1)
{
    char *p;
    unsigned short *q;

    p = param_1 + 0x14;
    if (func_0200b848(p) == 0) {
        return 0;
    }
    q = (unsigned short *)func_0200b854(p);
    if (q == 0) {
        return 0;
    }
    if (*q <= *(unsigned short *)(p + 0x14)) {
        return 0;
    }
    func_0200c64c((int)func_02008860(), (int)(param_1 + 0x14));
    return 1;
}
