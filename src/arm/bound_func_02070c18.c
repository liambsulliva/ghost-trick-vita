extern char data_020b1748;
extern char data_020d0e48;
extern int func_02071554();
extern int func_02072cc4();

void func_02070c18(void)

{
    int iVar1;

    if (*((int *)&data_020b1748) != -1) {
        func_02072cc4();
    }
    iVar1 = ((int)&data_020d0e48);
    func_02071554(*(unsigned int *)(((int)&data_020d0e48) + 0x14));
    *(unsigned int *)(iVar1 + 0x14) = 0;
    *(unsigned int *)(iVar1 + 0x10) = 0;
    *(unsigned int *)(iVar1 + 0xc) = 0;
    return;
}
