extern int func_02071a6c();
extern int func_02072cc4();

void func_02070dcc(void)

{
    unsigned int *puVar1;

    if (*((int *)0x020b1748u) != -1) {
        func_02072cc4();
    }
    puVar1 = ((unsigned int *)0x020d0e48u);
    func_02071a6c(*((unsigned int *)0x020d0e48u));
    *puVar1 = 0;
    return;
}
