extern int func_0202252c();
extern int func_0207dea4();

unsigned int * func_0200ca70(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020a2dccu);
    if (param_1[0xf] != 0) {
        func_0207dea4(param_1[0xf],0x18,8,((unsigned int)0x0200ca65u));
        param_1[0xf] = 0;
    }
    func_0202252c(param_1 + 1);
    return param_1;
}
