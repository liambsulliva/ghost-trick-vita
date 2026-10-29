extern int func_0202f644();
extern int func_02047198();
extern int func_0207dea4();

unsigned int * func_02048db8(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020a9b4cu);
    if (param_1[0x2e] != 0) {
        func_0207dea4(param_1[0x2e],200,8,((unsigned int)0x02056c8du));
        param_1[0x2e] = 0;
    }
    func_02047198(param_1);
    func_0202f644(param_1);
    return param_1;
}
