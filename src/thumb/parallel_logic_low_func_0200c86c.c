extern void func_0200cb94(int param_1);

void func_0200c86c(char *param_1, int param_2)
{
    int iVar1;

    iVar1 = *(int *)(param_1 + param_2 * 4 + 0x1510);
    if (iVar1 != 0) {
        func_0200cb94(iVar1);
    }
    iVar1 = *(int *)((int)param_1 + param_2 * 4 + 0x1518);
    if (iVar1 != 0) {
        func_0200cb94(iVar1);
    }
    iVar1 = *(int *)((unsigned int)param_1 + param_2 * 4 + 0x1520);
    if (iVar1 != 0) {
        func_0200cb94(iVar1);
    }
}
