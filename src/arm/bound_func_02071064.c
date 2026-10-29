extern char data_020b1748;
extern char data_020d0e64;
extern int func_020716e8();
extern int func_02072cc4();

void func_02071064(void)

{
    int iVar1;

    if (*((int *)&data_020b1748) != -1) {
        func_02072cc4();
    }
    iVar1 = ((int)&data_020d0e64);
    func_020716e8(*(unsigned int *)(((int)&data_020d0e64) + 0x14));
    *(unsigned int *)(iVar1 + 0x1c) = 0;
    *(unsigned int *)(iVar1 + 0x18) = 0;
    *(unsigned int *)(iVar1 + 4) = 0;
    *(unsigned int *)(iVar1 + 0x14) = 0;
    return;
}
