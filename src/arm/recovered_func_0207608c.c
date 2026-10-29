extern int func_02074120();
extern int func_02075c98();
extern int func_02075cac();

void func_0207608c(unsigned int *param_1,unsigned int param_2)

{
    unsigned int uVar1;

    uVar1 = func_02075c98();
    if (param_2 != 0) {
        *param_1 = *param_1 | param_2;
        func_02074120(param_1 + 1);
    }
    func_02075cac(uVar1);
    return;
}
