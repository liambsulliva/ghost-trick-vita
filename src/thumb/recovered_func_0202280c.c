extern int func_020227e0();

void func_0202280c(void)

{
    int iVar1;

    iVar1 = func_020227e0();
    if (iVar1 != 0) {
        *(unsigned int *)(iVar1 + 0xc) = ((unsigned int)0x00ffffffu) & *(unsigned int *)(iVar1 + 0xc);
    }
    return;
}
