extern int func_020293f8();
extern int func_0202f644();
extern int func_0207dea4();

unsigned int * func_0205c634(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020af158u);
    if (param_1[0x15] != 0) {
        func_0207dea4(param_1[0x15],0x44,8,((unsigned int)0x02028b29u));
        param_1[0x15] = 0;
    }
    func_020293f8(param_1);
    func_0202f644(param_1);
    return param_1;
}
