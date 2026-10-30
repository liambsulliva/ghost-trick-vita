extern int func_0207454c();
extern int func_02075c98();
extern void func_02075cac();
int func_02076fe4(void)
{
    int saved;
    unsigned int v;

    if (func_0207454c() == 0) {
        return 1;
    }
    saved = func_02075c98();
    *(volatile unsigned int *)0x04fff200 = 16;
    v = *(volatile unsigned int *)0x04fff200;
    func_02075cac(saved);
    if (v == 0) {
        return 0;
    }
    return 1;
}
