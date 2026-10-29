extern int func_0200b848();
extern int func_0200cd30();

unsigned int func_0200ba98(int param_1,unsigned int param_2)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_0200b848(param_1 + 0x14);
    uVar2 = 0;
    if (iVar1 != 0) {
        uVar2 = func_0200cd30(*(unsigned int *)(iVar1 + 0x10),param_2);
    }
    return uVar2;
}
