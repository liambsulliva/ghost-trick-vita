void func_0200a620(int param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;

    uVar2 = 0;
    do {
        uVar1 = uVar2 + 1;
        *(char *)(param_1 + uVar2) = (char)uVar1;
        uVar2 = uVar1;
    } while (uVar1 < 0xff);
    *(unsigned char *)(param_1 + 0xff) = 0;
    *(unsigned char *)(param_1 + 0x100) = 0;
    *(unsigned char *)(param_1 + 0x101) = 0xff;
    return;
}
