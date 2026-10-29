extern int func_0200b060();
extern int func_0200b074();
extern int func_0200b848();

void func_0200bbd0(int param_1)

{
    int iVar1;

    iVar1 = func_0200b848(param_1 + 0x14);
    if (iVar1 != 0) {
        func_0200b074();
        func_0200b060(iVar1);
    }
    return;
}
