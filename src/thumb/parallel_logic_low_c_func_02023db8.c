extern int func_0201e2cc();
extern int func_02023d9c();
extern int func_0201e2ec();
extern int func_0200c530();
extern int func_02007bd8();
extern int func_0201f288();
extern char data_02084fa0;

int func_02023db8(int param_1)
{
    int iVar1;

    func_0201e2cc(param_1);
    func_02023d9c(param_1 + 0x20, 0x280);
    func_0201e2ec(param_1, 0x40, 0xa0, 0, 0);
    func_0200c530(param_1, &data_02084fa0);
    iVar1 = func_02007bd8();
    func_0201f288(iVar1, param_1);
    return param_1;
}
