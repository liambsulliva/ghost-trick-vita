extern int func_02008444();
extern int func_02046490();
extern int func_02050a9c();

unsigned int * func_02051a2c(unsigned int *param_1)

{
    int iVar1;

    func_02050a9c();
    *param_1 = ((unsigned int)0x020abdbcu);
    func_02008444();
    iVar1 = func_02046490();
    param_1[1] = iVar1 + 0x34;
    return param_1;
}
