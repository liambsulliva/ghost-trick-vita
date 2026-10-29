extern int func_0201572c();

unsigned int func_0203efec(int param_1)

{
    int iVar1;

    if (*(char *)(param_1 + 5) != '\0') {
        *(char *)(param_1 + 5) = *(char *)(param_1 + 5) + -1;
        return 0;
    }
    iVar1 = func_0201572c();
    if (iVar1 == 0) {
        return 1;
    }
    return 0;
}
