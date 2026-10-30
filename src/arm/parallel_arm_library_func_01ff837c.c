extern unsigned short func_02073680();
extern void func_01ff848c();
extern void func_01ff83a8();

void func_01ff837c(void)
{
    unsigned short v;
    do {
        v = func_02073680();
    } while (v == 0);
    *(unsigned short *)0x04000208 = 0;
    func_01ff848c(0);
    func_01ff83a8();
}
