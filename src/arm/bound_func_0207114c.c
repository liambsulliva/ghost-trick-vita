extern char data_020b1748;
extern char data_020d0e64;
extern int func_02071860();
extern int func_02072cc4();

void func_0207114c(void)

{
    int iVar1;

    if (*((int *)&data_020b1748) != -1) {
        func_02072cc4();
    }
    iVar1 = ((int)&data_020d0e64);
    func_02071860(*(unsigned int *)(((int)&data_020d0e64) + 0xc));
    *(unsigned int *)(iVar1 + 0xc) = 0;
    *(unsigned int *)(iVar1 + 8) = 0;
    return;
}
