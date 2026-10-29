extern int func_0201786c();

void func_0201aa14(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    uVar2 = 0;
    do {
        iVar1 = uVar2 * 8;
        func_0201786c(*(unsigned int *)(param_1 + iVar1 + 0xe0),0xa00);
        *(unsigned short *)(param_1 + iVar1 + 0xe4) = 0;
        *(unsigned char *)(param_1 + iVar1 + 0xe6) = 0;
        *(unsigned char *)(param_1 + iVar1 + 0xe7) = 0;
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 3);
    return;
}
