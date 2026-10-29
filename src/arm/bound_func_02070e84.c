extern char data_020b1748;
extern char data_020d0e48;
extern int func_02071aec();
extern int func_02072cc4();

void func_02070e84(void)

{
    int iVar1;

    if (*((int *)&data_020b1748) != -1) {
        func_02072cc4();
    }
    iVar1 = ((int)&data_020d0e48);
    func_02071aec(*(unsigned int *)(((int)&data_020d0e48) + 0x18));
    *(unsigned int *)(iVar1 + 0x18) = 0;
    return;
}
