extern int func_0206981c();
extern int func_0206f7d0();

void func_020699cc(void)

{
    func_0206f7d0();
    func_0206981c();
    *((int *)0x04000600u) = *((int *)0x04000600u) & 0x3fffffff | 0x80000000;
    return;
}
