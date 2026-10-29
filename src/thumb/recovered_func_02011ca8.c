extern int func_02031270();

void func_02011ca8(int param_1)

{
    int iVar1;

    iVar1 = ((int)0x00001098u);
    if (*(int *)(param_1 + ((int)0x00001098u)) != 0) {
        func_02031270();
        *(unsigned int *)(param_1 + iVar1) = 0;
    }
    return;
}
