extern int func_0206c440();
extern int func_0206c7d0();

unsigned int func_0201efb4(int param_1,unsigned int param_2)

{
    int iVar1;

    *(unsigned int *)(param_1 + 4) = param_2;
    func_0206c440(param_1 + 8);
    iVar1 = func_0206c7d0(param_1 + 8,param_2);
    if (iVar1 == 0) {
        return 0;
    }
    return 1;
}
