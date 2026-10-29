extern int func_02008444();
extern int func_0202f644();
extern int func_02031b04();
extern int func_0204506c();

unsigned int * func_02045a4c(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)0x020a86a4u);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x58);
    func_0204506c(param_1);
    func_0202f644(param_1);
    return param_1;
}
