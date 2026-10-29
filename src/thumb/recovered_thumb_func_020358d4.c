extern int func_0201123c();
extern int func_02011264();
extern int func_02022fc0();

void func_020358d4(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,0);
    iVar2 = func_02022fc0(uVar1,((unsigned int)0x020cd9d8u));
    *(int *)(param_1 + 0x28) = iVar2;
    if (iVar2 != 0) {
        *(unsigned char *)(iVar2 + 0x3c) = 0x40;
        *(unsigned char *)(param_1 + 0x10) = 0;
        *(unsigned char *)(param_1 + 0x11) = 0;
    }
    return;
}
