extern int func_02008444();
extern int func_020113f8();
extern int func_02031af4();

void func_0204043c(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    if (*(char *)(param_1 + 4) == '\0') {
        uVar1 = func_02008444();
        iVar2 = func_02031af4(uVar1,*(unsigned int *)(param_1 + 0x24));
        if (iVar2 == 0) {
            func_02008444();
            func_020113f8();
            return;
        }
        *(unsigned char *)(param_1 + 4) = 1;
    }
    return;
}
