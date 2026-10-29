extern int func_0200b848();
extern int func_0200cd3c();

void func_0200bab8(int param_1,int param_2)

{
    int iVar1;

    if ((param_2 != -1) && (iVar1 = func_0200b848(param_1 + 0x14), iVar1 != 0)) {
        func_0200cd3c(*(unsigned int *)(iVar1 + 0x10),param_2);
    }
    return;
}
