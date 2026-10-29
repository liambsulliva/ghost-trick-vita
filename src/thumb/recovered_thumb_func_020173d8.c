extern int func_02070290();
extern int func_020702f4();
extern int func_0207487c();

void func_020173d8(int param_1)

{
    int iVar1;

    iVar1 = *(int *)(param_1 + 4);
    if (iVar1 != 0) {
        if (*(char *)(param_1 + 0x22) == '\x01') {
            func_0207487c(iVar1,*(int *)(param_1 + 8) << 3);
            func_020702f4(*(unsigned int *)(param_1 + 4),0,*(int *)(param_1 + 8) << 3);
            return;
        }
        func_0207487c(iVar1,*(int *)(param_1 + 8) << 3);
        func_02070290(*(unsigned int *)(param_1 + 4),0,*(int *)(param_1 + 8) << 3);
    }
    return;
}
