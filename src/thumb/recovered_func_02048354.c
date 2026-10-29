extern int func_02031288();
extern int func_02047120();

unsigned int * func_02048354(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02047120();
    *param_1 = ((unsigned int)0x020a8ebcu);
    uVar1 = func_02031288(0xa00);
    param_1[0x2e] = uVar1;
    return param_1;
}
