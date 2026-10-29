extern char data_020b1760;
extern int func_02074534();

void func_02074500(void)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((int)&data_020b1760);
    if (*(int *)(((int)&data_020b1760) + 4) == -1) {
        *(unsigned int *)(((int)&data_020b1760) + 4) = 0x80000001;
        uVar2 = func_02074534();
        *(unsigned int *)(iVar1 + 4) = *(unsigned int *)(iVar1 + 4) | uVar2;
        return;
    }
    return;
}
