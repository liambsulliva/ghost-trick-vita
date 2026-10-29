extern int func_0204f1cc();
extern int func_0204f1d8();
extern int func_0204f1e4();
extern int func_0204f204();
extern int func_0204f220();

void func_0204f17c(int param_1)

{
    int iVar1;
    int iVar2;

    if (*(signed char *)(param_1 + 0x44) >= 1) {
        func_0204f1cc(1);
        iVar1 = 0;
        do {
            func_0204f1d8((unsigned int)*(unsigned short *)(param_1 + iVar1 * 2 + 0x9c));
            iVar2 = param_1 + iVar1 * 8;
            func_0204f1e4(*(int *)(iVar2 + 0xa4),*(int *)(iVar2 + 0xa8));
            func_0204f204((int)*(short *)(iVar2 + 0x7c),(int)*(short *)(iVar2 + 0x7e),0);
            iVar1 = iVar1 + 1;
        } while (iVar1 < 4);
        func_0204f220();
    }
    return;
}
