extern char data_020b1748;
extern char data_020d0e48;
extern int func_0207164c();
extern int func_02072cc4();

void func_02070d10(void)

{
    int iVar1;

    if (*((int *)&data_020b1748) != -1) {
        func_02072cc4();
    }
    iVar1 = ((int)&data_020d0e48);
    func_0207164c(*(unsigned int *)(((int)&data_020d0e48) + 8));
    *(unsigned int *)(iVar1 + 8) = 0;
    *(unsigned int *)(iVar1 + 4) = 0;
    return;
}
