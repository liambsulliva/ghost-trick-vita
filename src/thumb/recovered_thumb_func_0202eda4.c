extern int func_0202edd0();
extern int func_0202ee20();

void func_0202eda4(unsigned int param_1,unsigned int param_2)

{
    int iVar1;
    int iVar2;

    iVar1 = func_0202edd0();
    iVar2 = func_0202edd0(param_2);
    if ((iVar1 != iVar2) && ((iVar1 == 2 || (iVar2 == 2)))) {
        func_0202ee20(param_1);
        func_0202ee20(param_2);
    }
    return;
}
