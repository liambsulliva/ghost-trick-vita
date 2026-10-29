extern int func_0202f64c();

void func_020437c4(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = 0;
    do {
        iVar2 = uVar1 * 0x10;
        if ((*(short *)(param_1 + iVar2 + 0x54) == 0x100) ||
              (*(unsigned short *)(param_1 + iVar2 + 0x56) == ((unsigned int)0x0000ffffu))) {
            func_0202f64c(param_1 + 0x48 + iVar2);
            func_0202f64c(param_1 + 0x4c + iVar2);
            func_0202f64c(param_1 + 0x50 + iVar2);
        }
        uVar1 = uVar1 + 1;
    } while (uVar1 < 5);
    return;
}
