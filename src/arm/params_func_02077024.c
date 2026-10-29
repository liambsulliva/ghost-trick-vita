extern char data_020d2e20;
void func_02077024(void)

{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar1 = ((int)&data_020d2e20);
    iVar3 = 0;
    do {
        *(unsigned int *)(iVar1 + iVar3 * 0xc) = 0;
        iVar2 = iVar1 + iVar3 * 0xc;
        *(unsigned int *)(iVar2 + 4) = 0;
        iVar3 = iVar3 + 1;
        *(unsigned char *)(iVar2 + 8) = 0;
    } while (iVar3 < 8);
    return;
}
