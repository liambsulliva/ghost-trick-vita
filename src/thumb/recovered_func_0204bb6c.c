extern int func_02008444();
extern int func_02031b04();
extern int func_02047198();

unsigned int * func_0204bb6c(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)0x020a9610u);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x2f);
    func_02047198(param_1);
    return param_1;
}
