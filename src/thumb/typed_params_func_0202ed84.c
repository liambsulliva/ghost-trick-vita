extern int func_0202eca8();
extern int func_0202ecf8();
extern int func_0202eda4();

void func_0202ed84(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;

    *(unsigned int *)(param_1 + 8) = param_2;
    func_0202eda4(param_1,param_3);
    uVar1 = func_0202eca8(param_3);
    func_0202ecf8(param_1,uVar1);
    return;
}
