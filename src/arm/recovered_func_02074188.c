extern int func_02073cc4();
extern int func_02075c98();
extern int func_02075cac();

void func_02074188(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02075c98();
    *(unsigned int *)(param_1 + 100) = 1;
    func_02073cc4();
    func_02075cac(uVar1);
    return;
}
