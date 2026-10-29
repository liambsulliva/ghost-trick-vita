extern int func_0201123c();
extern int func_02011264();
extern int func_0201572c();
extern int func_02022f34();
extern int func_0203d670();

void func_0203d5d0(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_0203d670();
    if (((iVar1 == 0) && (*(char *)(param_1 + 4) == '\x02')) && (iVar1 = func_0201572c(), iVar1 == 0))
    {
        uVar2 = func_02011264();
        uVar2 = func_0201123c(uVar2,0);
        func_02022f34(uVar2,((unsigned int)0x0209ab34u),0);
    }
    return;
}
