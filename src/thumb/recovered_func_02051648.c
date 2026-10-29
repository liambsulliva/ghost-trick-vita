extern int func_02008444();
extern int func_02046490();
extern int func_02050a9c();

unsigned int * func_02051648(unsigned int *param_1)

{
    int iVar1;

    func_02050a9c();
    *param_1 = ((unsigned int)0x020abec4u);
    func_02008444();
    iVar1 = func_02046490();
    param_1[1] = iVar1 + 0x34;
    return param_1;
}
