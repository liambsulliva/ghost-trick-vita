extern int func_0201f934();

void func_0201fd60(int param_1,int param_2)

{
    int iVar1;

    iVar1 = func_0201f934();
    if ((iVar1 != 0) && (iVar1 = *(int *)(*(int *)(param_1 + 0xc) + param_2 * 4), iVar1 != 0)) {
        *(unsigned char *)(iVar1 + 6) = *(unsigned char *)(iVar1 + 6) & ~0x2;
    }
    return;
}
