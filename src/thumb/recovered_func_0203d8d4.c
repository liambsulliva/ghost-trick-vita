extern int func_0201572c();
extern int func_02022f6c();
extern int func_0203d670();

void func_0203d8d4(int param_1)

{
    int iVar1;

    iVar1 = func_0203d670();
    if (((iVar1 == 0) && (*(char *)(param_1 + 4) == '\x02')) && (iVar1 = func_0201572c(), iVar1 == 0))
    {
        func_02022f6c(param_1);
    }
    return;
}
