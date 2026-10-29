extern int func_0200db3c();
extern int func_02027c28();

void func_0200d9d0(int param_1,unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = func_02027c28(param_2);
    *(unsigned int *)(param_1 + 4) = uVar1;
    func_0200db3c(param_1);
    return;
}
